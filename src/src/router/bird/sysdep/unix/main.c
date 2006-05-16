/*
 *	BIRD Internet Routing Daemon -- Unix Entry Point
 *
 *	(c) 1998--2000 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#undef LOCAL_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "nest/bird.h"
#include "lib/lists.h"
#include "lib/resource.h"
#include "lib/socket.h"
#include "lib/event.h"
#include "lib/string.h"
#include "nest/route.h"
#include "nest/protocol.h"
#include "nest/iface.h"
#include "nest/cli.h"
#include "nest/locks.h"
#include "conf/conf.h"
#include "filter/filter.h"

#include "unix.h"
#include "krt.h"

/*
 *	Debugging
 */

#ifdef DEBUGGING
static int debug_flag = 1;
#else
static int debug_flag = 0;
#endif

void
async_dump(void)
{
  bdebug("INTERNAL STATE DUMP\n\n");

  rdump(&root_pool);
  sk_dump_all();
  tm_dump_all();
  if_dump_all();
  neigh_dump_all();
  rta_dump_all();
  rt_dump_all();
  protos_dump_all();

  bdebug("\n");
}

/*
 *	Reading the Configuration
 */

static int conf_fd;
static char *config_name = PATH_CONFIG;

static int
cf_read(byte *dest, unsigned int len)
{
  int l = read(conf_fd, dest, len);
  if (l < 0)
    cf_error("Read error");
  return l;
}

void
sysdep_preconfig(struct config *c)
{
  init_list(&c->logfiles);
}

int
sysdep_commit(struct config *new, struct config *old UNUSED)
{
  log_switch(debug_flag, &new->logfiles);
  return 0;
}

static int
unix_read_config(struct config **cp, char *name)
{
  struct config *conf = config_alloc(name);

  *cp = conf;
  conf_fd = open(name, O_RDONLY);
  if (conf_fd < 0)
    return 0;
  cf_read_hook = cf_read;
  return config_parse(conf);
}

static void
read_config(void)
{
  struct config *conf;

  if (!unix_read_config(&conf, config_name))
    {
      if (conf->err_msg)
	die("%s, line %d: %s", config_name, conf->err_lino, conf->err_msg);
      else
	die("Unable to open configuration file %s: %m", config_name);
    }
  config_commit(conf);
}

void
async_config(void)
{
  struct config *conf;

  log(L_INFO "Reconfiguration requested by SIGHUP");
  if (!unix_read_config(&conf, config_name))
    {
      if (conf->err_msg)
	log(L_ERR "%s, line %d: %s", config_name, conf->err_lino, conf->err_msg);
      else
	log(L_ERR "Unable to open configuration file %s: %m", config_name);
      config_free(conf);
    }
  else
    config_commit(conf);
}

void
cmd_reconfig(char *name)
{
  struct config *conf;

  if (!name)
    name = config_name;
  cli_msg(-2, "Reading configuration from %s", name);
  if (!unix_read_config(&conf, name))
    {
      if (conf->err_msg)
	cli_msg(8002, "%s, line %d: %s", name, conf->err_lino, conf->err_msg);
      else
	cli_msg(8002, "%s: %m", name);
      config_free(conf);
    }
  else
    {
      switch (config_commit(conf))
	{
	case CONF_DONE:
	  cli_msg(3, "Reconfigured.");
	  break;
	case CONF_PROGRESS:
	  cli_msg(4, "Reconfiguration in progress.");
	  break;
	case CONF_SHUTDOWN:
	  cli_msg(6, "Reconfiguration ignored, shutting down.");
	  break;
	default:
	  cli_msg(5, "Reconfiguration already in progress, queueing new config");
	}
    }
}

/*
 *	Command-Line Interface
 */

static sock *cli_sk;
static char *path_control_socket = PATH_CONTROL_SOCKET;

int
cli_write(cli *c)
{
  sock *s = c->priv;

  if (c->tx_pos)
    {
      struct cli_out *o = c->tx_pos;
      s->tbuf = o->outpos;
      if (sk_send(s, o->wpos - o->outpos) > 0)
	{
	  c->tx_pos = o->next;
	  ev_schedule(c->event);
	}
    }
  return !c->tx_pos;
}

int
cli_get_command(cli *c)
{
  sock *s = c->priv;
  byte *t = c->rx_aux ? : s->rbuf;
  byte *tend = s->rpos;
  byte *d = c->rx_pos;
  byte *dend = c->rx_buf + CLI_RX_BUF_SIZE - 2;

  while (t < tend)
    {
      if (*t == '\r')
	t++;
      else if (*t == '\n')
	{
	  t++;
	  c->rx_pos = c->rx_buf;
	  c->rx_aux = t;
	  *d = 0;
	  return (d < dend) ? 1 : -1;
	}
      else if (d < dend)
	*d++ = *t++;
    }
  c->rx_aux = s->rpos = s->rbuf;
  c->rx_pos = d;
  return 0;
}

static int
cli_rx(sock *s, int size UNUSED)
{
  cli_kick(s->data);
  return 0;
}

static void
cli_tx(sock *s)
{
  cli *c = s->data;

  if (cli_write(c))
    cli_written(c);
}

static void
cli_err(sock *s, int err)
{
  if (config->cli_debug)
    {
      if (err)
	log(L_INFO "CLI connection dropped: %s", strerror(err));
      else
	log(L_INFO "CLI connection closed");
    }
  cli_free(s->data);
}

static int
cli_connect(sock *s, int size UNUSED)
{
  cli *c;

  if (config->cli_debug)
    log(L_INFO "CLI connect");
  s->rx_hook = cli_rx;
  s->tx_hook = cli_tx;
  s->err_hook = cli_err;
  s->data = c = cli_new(s);
  s->pool = c->pool;		/* We need to have all the socket buffers allocated in the cli pool */
  c->rx_pos = c->rx_buf;
  c->rx_aux = NULL;
  rmove(s, c->pool);
  return 1;
}

static void
cli_init_unix(void)
{
  sock *s;

  cli_init();
  s = cli_sk = sk_new(cli_pool);
  s->type = SK_UNIX_PASSIVE;
  s->rx_hook = cli_connect;
  s->rbsize = 1024;
  if (sk_open_unix(s, path_control_socket) < 0)
    die("Unable to create control socket %s", path_control_socket);
}

/*
 *	Shutdown
 */

void
async_shutdown(void)
{
  DBG("Shutting down...\n");
  order_shutdown();
}

void
sysdep_shutdown_done(void)
{
  unlink(PATH_CONTROL_SOCKET);
  die("System shutdown completed");
}

/*
 *	Signals
 */

static void
handle_sighup(int sig UNUSED)
{
  DBG("Caught SIGHUP...\n");
  async_config_flag = 1;
}

static void
handle_sigusr(int sig UNUSED)
{
  DBG("Caught SIGUSR...\n");
  async_dump_flag = 1;
}

static void
handle_sigterm(int sig UNUSED)
{
  DBG("Caught SIGTERM...\n");
  async_shutdown_flag = 1;
}

static void
signal_init(void)
{
  struct sigaction sa;

  bzero(&sa, sizeof(sa));
  sa.sa_handler = handle_sigusr;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &sa, NULL);
  sa.sa_handler = handle_sighup;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGHUP, &sa, NULL);
  sa.sa_handler = handle_sigterm;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &sa, NULL);
  signal(SIGPIPE, SIG_IGN);
}

/*
 *	Parsing of command-line arguments
 */

static char *opt_list = "c:dD:s:";

static void
usage(void)
{
  fprintf(stderr, "Usage: bird [-c <config-file>] [-d] [-D <debug-file>] [-s <control-socket>]\n");
  exit(1);
}

static void
parse_args(int argc, char **argv)
{
  int c;

  if (argc == 2)
    {
      if (!strcmp(argv[1], "--version"))
	{
	  fprintf(stderr, "BIRD version " BIRD_VERSION "\n");
	  exit(0);
	}
      if (!strcmp(argv[1], "--help"))
	usage();
    }
  while ((c = getopt(argc, argv, opt_list)) >= 0)
    switch (c)
      {
      case 'c':
	config_name = optarg;
	break;
      case 'd':
	debug_flag |= 1;
	break;
      case 'D':
	log_init_debug(optarg);
	debug_flag |= 2;
	break;
      case 's':
	path_control_socket = optarg;
	break;
      default:
	usage();
      }
  if (optind < argc)
    usage();
}

/*
 *	Hic Est main()
 */

int
bird_main(int argc, char **argv)
{
#ifdef HAVE_LIBDMALLOC
  if (!getenv("DMALLOC_OPTIONS"))
    dmalloc_debug(0x2f03d00);
#endif

  parse_args(argc, argv);
  if (debug_flag == 1)
    log_init_debug("");
  log_init(debug_flag, 1);

  test_old_bird(path_control_socket);

  DBG("Initializing.\n");
  resource_init();
  olock_init();
  io_init();
  rt_init();
  if_init();

  protos_build();
  proto_build(&proto_unix_kernel);
  proto_build(&proto_unix_iface);

  read_config();

  if (!debug_flag)
    {
      pid_t pid = fork();
      if (pid < 0)
	die("fork: %m");
      if (pid)
	return 0;
      setsid();
    }

  signal_init();

  cli_init_unix();

#ifdef LOCAL_DEBUG
  async_dump_flag = 1;
#endif

  DBG("Entering I/O loop.\n");

  io_loop();
  bug("I/O loop died");
}
