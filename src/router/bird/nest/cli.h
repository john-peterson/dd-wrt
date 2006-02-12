/*
 *	BIRD Internet Routing Daemon -- Command-Line Interface
 *
 *	(c) 1999--2000 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#ifndef _BIRD_CLI_H_
#define _BIRD_CLI_H_

#include "lib/resource.h"
#include "lib/event.h"

#define CLI_RX_BUF_SIZE 4096
#define CLI_TX_BUF_SIZE 4096
#define CLI_MAX_ASYNC_QUEUE 4096

struct cli_out {
  struct cli_out *next;
  byte *wpos, *outpos, *end;
  byte buf[0];
};

typedef struct cli {
  node n;				/* Node in list of all log hooks */
  pool *pool;
  void *priv;				/* Private to sysdep layer */
  byte *rx_buf, *rx_pos, *rx_aux;	/* sysdep */
  struct cli_out *tx_buf, *tx_pos, *tx_write;
  event *event;
  void (*cont)(struct cli *c);
  void (*cleanup)(struct cli *c);
  void *rover;				/* Private to continuation routine */
  int last_reply;
  struct linpool *parser_pool;		/* Pool used during parsing */
  byte *ring_buf;			/* Ring buffer for asynchronous messages */
  byte *ring_end, *ring_read, *ring_write;	/* Pointers to the ring buffer */
  unsigned int ring_overflow;		/* Counter of ring overflows */
  unsigned int log_mask;		/* Mask of allowed message levels */
  unsigned int log_threshold;		/* When free < log_threshold, store only important messages */
  unsigned int async_msg_size;		/* Total size of async messages queued in tx_buf */
} cli;

extern pool *cli_pool;
extern struct cli *this_cli;		/* Used during parsing */

/* Functions to be called by command handlers */

void cli_printf(cli *, int, char *, ...);
#define cli_msg(x...) cli_printf(this_cli, x)
void cli_set_log_echo(cli *, unsigned int mask, unsigned int size);

/* Functions provided to sysdep layer */

cli *cli_new(void *);
void cli_init(void);
void cli_free(cli *);
void cli_kick(cli *);
void cli_written(cli *);
void cli_echo(unsigned int class, byte *msg);

/* Functions provided by sysdep layer */

int cli_write(cli *);
int cli_get_command(cli *);

#endif
