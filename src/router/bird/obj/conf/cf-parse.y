%{
/* Headers from ../../conf/confbase.Y */

#include "nest/bird.h"
#include "conf/conf.h"
#include "lib/resource.h"
#include "lib/socket.h"
#include "lib/timer.h"
#include "lib/string.h"
#include "nest/protocol.h"
#include "nest/iface.h"
#include "nest/route.h"
#include "nest/cli.h"
#include "filter/filter.h"

/* FIXME: Turn on YYERROR_VERBOSE and work around lots of bison bugs? */

/* Headers from ../../sysdep/unix/config.Y */

#include "lib/unix.h"
#include <stdio.h>

/* Headers from ../../sysdep/unix/krt.Y */

#include "lib/krt.h"

/* Defines from ../../sysdep/unix/krt.Y */

#define THIS_KRT ((struct krt_config *) this_proto)
#define THIS_KIF ((struct kif_config *) this_proto)

/* Headers from ../../sysdep/linux/netlink/netlink.Y */

/* Headers from ../../nest/config.Y */

#include "nest/rt-dev.h"
#include "nest/password.h"
#include "nest/cmds.h"
#include "lib/lists.h"

/* Defines from ../../nest/config.Y */

static struct proto_config *this_proto;
static struct iface_patt *this_ipatt;
static struct iface_patt_node *this_ipn;
static list *this_p_list;
static struct password_item *this_p_item;
static int password_id;

static inline void
reset_passwords(void)
{
 this_p_list = NULL;
}

static inline list *
get_passwords(void)
{
  list *rv = this_p_list;
  this_p_list = NULL;
  return rv;
}


/* Headers from ../../filter/config.Y */

/* Defines from ../../filter/config.Y */

#define P(a,b) ((a<<8) | b)

static int make_pair(int i1, int i2)
{
  unsigned u1 = i1;
  unsigned u2 = i2;

  if ((u1 > 0xFFFF) || (u2 > 0xFFFF))
    cf_error( "Can't operate with value out of bounds in pair constructor");

  return (u1 << 16) | u2;
}

/* Headers from ../../proto/bgp/config.Y */

#include "proto/bgp/bgp.h"

/* Defines from ../../proto/bgp/config.Y */

#define BGP_CFG ((struct bgp_config *) this_proto)

/* Headers from ../../proto/ospf/config.Y */

#include "proto/ospf/ospf.h"

/* Defines from ../../proto/ospf/config.Y */

#define OSPF_CFG ((struct ospf_config *) this_proto)
#define OSPF_PATT ((struct ospf_iface_patt *) this_ipatt)

static struct ospf_area_config *this_area;
static struct nbma_node *this_nbma;
static struct area_net_config *this_pref;
static struct ospf_stubnet_config *this_stubnet; 

#ifdef OSPFv2
static void
finish_iface_config(struct ospf_iface_patt *ip)
{
  ip->passwords = get_passwords();

  if ((ip->autype == OSPF_AUTH_CRYPT) && (ip->helloint < 5))
    log(L_WARN "Hello or poll interval less that 5 makes cryptographic authenication prone to replay attacks");

  if ((ip->autype == OSPF_AUTH_NONE) && (ip->passwords != NULL))
    log(L_WARN "Password option without authentication option does not make sense");
}
#endif

#ifdef OSPFv3
static void
finish_iface_config(struct ospf_iface_patt *ip)
{
  if ((ip->autype != OSPF_AUTH_NONE) || (get_passwords() != NULL))
    cf_error("Authentication not supported in OSPFv3");
}
#endif

/* Headers from ../../proto/pipe/config.Y */

#include "proto/pipe/pipe.h"

/* Defines from ../../proto/pipe/config.Y */

#define PIPE_CFG ((struct pipe_config *) this_proto)

/* Headers from ../../proto/rip/config.Y */

#include "proto/rip/rip.h"
#include "nest/iface.h"

/* Defines from ../../proto/rip/config.Y */

#define RIP_CFG ((struct rip_proto_config *) this_proto)
#define RIP_IPATT ((struct rip_patt *) this_ipatt)

/* Headers from ../../proto/static/config.Y */

#include "proto/static/static.h"

/* Defines from ../../proto/static/config.Y */

static struct static_route *this_srt;

%}

/* Declarations from ../../conf/confbase.Y */

%union {
  int i;
  u32 i32;
  ip_addr a;
  struct symbol *s;
  char *t;
  struct rtable_config *r;
  struct f_inst *x;
  struct filter *f;
  struct f_tree *e;
  struct f_trie *trie;
  struct f_val v;
  struct f_path_mask *h;
  struct password_item *p;
  struct rt_show_data *ra;
  void *g;
  bird_clock_t time;
  struct prefix px;
  struct proto_spec ps;
  struct timeformat *tf;
}

%token END CLI_MARKER INVALID_TOKEN
%token GEQ LEQ NEQ AND OR
%token PO PC
%token <i> NUM ENUM
%token <i32> RTRID
%token <a> IPA
%token <s> SYM
%token <t> TEXT

%type <i> expr bool pxlen
%type <time> datetime
%type <a> ipa
%type <px> prefix prefix_or_ipa
%type <t> text_or_none

%nonassoc PREFIX_DUMMY
%left AND OR
%nonassoc '=' '<' '>' '~' '.' GEQ LEQ NEQ PO PC
%left '+' '-'
%left '*' '/' '%'
%left '!'

%token DEFINE ON OFF YES NO

/* Declarations from ../../sysdep/unix/config.Y */

%token LOG SYSLOG ALL DEBUG TRACE INFO REMOTE WARNING ERROR AUTH FATAL BUG STDERR SOFT
%token TIMEFORMAT ISO SHORT LONG BASE NAME

%type <i> log_mask log_mask_list log_cat
%type <g> log_file
%type <t> cfg_name
%type <tf> timeformat_which
%type <t> syslog_name

%token CONFIGURE
%token DOWN
/* Declarations from ../../sysdep/unix/krt.Y */

%token KERNEL PERSIST SCAN TIME LEARN DEVICE ROUTES

/* Declarations from ../../sysdep/linux/netlink/netlink.Y */

%token ASYNC TABLE

/* Declarations from ../../nest/config.Y */

%token ROUTER ID PROTOCOL PREFERENCE DISABLED DIRECT
%token INTERFACE IMPORT EXPORT FILTER NONE STATES FILTERS
%token PASSWORD FROM PASSIVE TO EVENTS PACKETS PROTOCOLS INTERFACES
%token PRIMARY STATS COUNT FOR COMMANDS PREEXPORT GENERATE
%token LISTEN BGP V6ONLY ADDRESS PORT PASSWORDS DESCRIPTION
%token RELOAD IN OUT MRTDUMP MESSAGES RESTRICT MEMORY






%type <i32> idval
%type <f> imexport
%type <r> rtable
%type <s> optsym
%type <ra> r_args
%type <i> echo_mask echo_size debug_mask debug_list debug_flag mrtdump_mask mrtdump_list mrtdump_flag export_or_preexport
%type <ps> proto_patt proto_patt2

%token SHOW STATUS
%token SUMMARY
%token ROUTE
%token SYMBOLS
%token DUMP RESOURCES
%token SOCKETS
%token NEIGHBORS
%token ATTRIBUTES
%token ECHO
%token DISABLE
%token ENABLE
%token RESTART
/* Declarations from ../../filter/config.Y */

%token FUNCTION PRINT PRINTN UNSET RETURN ACCEPT REJECT QUITBIRD INT BOOL IP PREFIX PAIR QUAD SET STRING BGPMASK BGPPATH CLIST IF THEN ELSE CASE TRUE FALSE GW NET MASK PROTO SOURCE SCOPE CAST DEST LEN DEFINED ADD DELETE CONTAINS RESET PREPEND FIRST LAST MATCH EMPTY WHERE EVAL

%nonassoc THEN
%nonassoc ELSE

%type <x> term block cmds cmds_int cmd function_body constant print_one print_list var_list var_listn dynamic_attr static_attr function_call symbol dpair bgp_path_expr
%type <f> filter filter_body where_filter
%type <i> type break_command cpair
%type <e> set_item set_items switch_body
%type <trie> fprefix_set
%type <v> set_atom fprefix fprefix_s fipa
%type <s> decls declsn one_decl function_params 
%type <h> bgp_path bgp_path_tail1 bgp_path_tail2

/* Declarations from ../../proto/bgp/config.Y */

%token LOCAL NEIGHBOR AS HOLD CONNECT RETRY KEEPALIVE MULTIHOP STARTUP VIA NEXT HOP SELF DEFAULT PATH METRIC START DELAY FORGET WAIT AFTER BGP_PATH BGP_LOCAL_PREF BGP_MED BGP_ORIGIN BGP_NEXT_HOP BGP_ATOMIC_AGGR BGP_AGGREGATOR BGP_COMMUNITY RR RS CLIENT CLUSTER AS4 ADVERTISE IPV4 CAPABILITIES LIMIT PREFER OLDER MISSING LLADDR DROP IGNORE REFRESH INTERPRET COMMUNITIES BGP_ORIGINATOR_ID BGP_CLUSTER_LIST

/* Declarations from ../../proto/ospf/config.Y */

%token OSPF AREA OSPF_METRIC1 OSPF_METRIC2 OSPF_TAG OSPF_ROUTER_ID
%token BROADCAST RFC1583COMPAT STUB TICK COST RETRANSMIT
%token HELLO TRANSMIT PRIORITY DEAD NONBROADCAST POINTOPOINT TYPE
%token SIMPLE AUTHENTICATION STRICT CRYPTOGRAPHIC
%token ELIGIBLE POLL NETWORKS HIDDEN VIRTUAL LINK
%token RX BUFFER LARGE NORMAL STUBNET
%token LSADB

%type <t> opttext

%token TOPOLOGY
%token STATE
/* Declarations from ../../proto/pipe/config.Y */

%token PIPE PEER MODE OPAQUE TRANSPARENT

/* Declarations from ../../proto/rip/config.Y */

%token RIP INFINITY PERIOD GARBAGE TIMEOUT MULTICAST QUIET NOLISTEN VERSION1 PLAINTEXT MD5 HONOR NEVER ALWAYS RIP_METRIC RIP_TAG

%type <i> rip_mode rip_auth

/* Declarations from ../../proto/static/config.Y */

%token STATIC PROHIBIT


%%
/* Grammar from ../../conf/confbase.Y */

/* Basic config file structure */

config: conf_entries END { return 0; }
 | CLI_MARKER cli_cmd { return 0; }
 ;

conf_entries:
   /* EMPTY */
 | conf_entries conf
 ;


/* Constant expressions */

expr:
   NUM
 | '(' term ')' { $$ = f_eval_int($2); }
 | SYM { if ($1->class != SYM_NUMBER) cf_error("Number expected"); else $$ = $1->aux; }
 ;

definition:
   DEFINE SYM '=' expr ';' {
     cf_define_symbol($2, SYM_NUMBER, NULL);
     $2->aux = $4;
   }
 | DEFINE SYM '=' IPA ';' {
     cf_define_symbol($2, SYM_IPA, cfg_alloc(sizeof(ip_addr)));
     *(ip_addr *)$2->def = $4;
   }
 ;

/* Switches */

bool:
   expr {$$ = !!$1; }
 | ON { $$ = 1; }
 | YES { $$ = 1; }
 | OFF { $$ = 0; }
 | NO { $$ = 0; }
 | /* Silence means agreement */ { $$ = 1; }
 ;

/* Addresses, prefixes and netmasks */

ipa:
   IPA
 | SYM {
     if ($1->class != SYM_IPA) cf_error("IP address expected");
     $$ = *(ip_addr *)$1->def;
   }
 ;

prefix:
   ipa pxlen {
     if (!ip_is_prefix($1, $2)) cf_error("Invalid prefix");
     $$.addr = $1; $$.len = $2;
   }
 ;

prefix_or_ipa:
   prefix
 | ipa { $$.addr = $1; $$.len = BITS_PER_IP_ADDRESS; }
 ;

pxlen:
   '/' expr {
     if ($2 < 0 || $2 > BITS_PER_IP_ADDRESS) cf_error("Invalid prefix length %d", $2);
     $$ = $2;
   }
 | ':' ipa {
     $$ = ipa_mklen($2);
     if ($$ < 0) cf_error("Invalid netmask %I", $2);
   }
 ;

datetime:
   TEXT {
     $$ = tm_parse_datetime($1);
     if (!$$)
       cf_error("Invalid date and time");
   }
 ;

text_or_none:
   TEXT { $$ = $1; }
 |      { $$ = NULL; }
 ;

/* Grammar from ../../sysdep/unix/config.Y */


log_config: LOG log_file log_mask ';' {
    struct log_config *c = cfg_allocz(sizeof(struct log_config));
    c->fh = $2;
    c->mask = $3;
    add_tail(&new_config->logfiles, &c->n);
  }
 ;

syslog_name:
   NAME TEXT { $$ = $2; }
 | { $$ = bird_name; }
 ;

log_file:
   TEXT {
     FILE *f = tracked_fopen(new_config->pool, $1, "a");
     if (!f) cf_error("Unable to open log file `%s': %m", $1);
     $$ = f;
   }
 | SYSLOG syslog_name { $$ = NULL; new_config->syslog_name = $2; }
 | STDERR { $$ = stderr; }
 ;

log_mask:
   ALL { $$ = ~0; }
 | '{' log_mask_list '}' { $$ = $2; }
 ;

log_mask_list:
   log_cat { $$ = 1 << $1; }
 | log_mask_list ',' log_cat { $$ = $1 | (1 << $3); }
 ;

log_cat:
   DEBUG { $$ = L_DEBUG[0]; }
 | TRACE { $$ = L_TRACE[0]; }
 | INFO { $$ = L_INFO[0]; }
 | REMOTE { $$ = L_REMOTE[0]; }
 | WARNING { $$ = L_WARN[0]; }
 | ERROR { $$ = L_ERR[0]; }
 | AUTH { $$ = L_AUTH[0]; }
 | FATAL { $$ = L_FATAL[0]; }
 | BUG { $$ = L_BUG[0]; }
 ;



mrtdump_base:
   MRTDUMP PROTOCOLS mrtdump_mask ';' { new_config->proto_default_mrtdump = $3; }
 | MRTDUMP TEXT ';' {
     FILE *f = tracked_fopen(new_config->pool, $2, "a");
     if (!f) cf_error("Unable to open MRTDump file '%s': %m", $2);
     new_config->mrtdump_file = fileno(f);
   }
 ;


timeformat_which:
   ROUTE { $$ = &new_config->tf_route; }
 | PROTOCOL { $$ = &new_config->tf_proto; }
 | BASE { $$ = &new_config->tf_base; }
 | LOG { $$ = &new_config->tf_log; }

timeformat_spec:
   timeformat_which TEXT { *$1 = (struct timeformat){$2, NULL, 0}; }
 | timeformat_which TEXT expr TEXT { *$1 = (struct timeformat){$2, $4, $3}; }
 | timeformat_which ISO SHORT { *$1 = (struct timeformat){"%T", "%F", 20*3600}; }
 | timeformat_which ISO LONG  { *$1 = (struct timeformat){"%F %T", NULL, 0}; }
 ;

timeformat_base:
   TIMEFORMAT timeformat_spec ';'
 ;

/* Unix specific commands */



cmd_CONFIGURE: CONFIGURE cfg_name END
{ cmd_reconfig($2, RECONFIG_HARD); } ;

cmd_CONFIGURE_SOFT: CONFIGURE SOFT cfg_name END
{ cmd_reconfig($3, RECONFIG_SOFT); } ;

cmd_DOWN: DOWN  END
{ cmd_shutdown(); } ;

cfg_name:
   /* empty */ { $$ = NULL; }
 | TEXT
 ;

/* Grammar from ../../sysdep/unix/krt.Y */

/* Kernel syncer protocol */


kern_proto_start: proto_start KERNEL {
#ifndef CONFIG_MULTIPLE_TABLES
     if (cf_krt)
       cf_error("Kernel protocol already defined");
#endif
     cf_krt = this_proto = proto_config_new(&proto_unix_kernel, sizeof(struct krt_config));
     this_proto->preference = DEF_PREF_INHERITED;
     THIS_KRT->scan_time = 60;
     THIS_KRT->learn = THIS_KRT->persist = 0;
     krt_scan_construct(THIS_KRT);
     krt_set_construct(THIS_KRT);
   }
 ;


kern_item:
   PERSIST bool { THIS_KRT->persist = $2; }
 | SCAN TIME expr {
      /* Scan time of 0 means scan on startup only */
      THIS_KRT->scan_time = $3;
   }
 | LEARN bool {
      THIS_KRT->learn = $2;
#ifndef KRT_ALLOW_LEARN
      if ($2)
	cf_error("Learning of kernel routes not supported in this configuration");
#endif
   }
 | DEVICE ROUTES bool { THIS_KRT->devroutes = $3; }
 ;

/* Kernel interface protocol */


kif_proto_start: proto_start DEVICE {
     if (cf_kif)
       cf_error("Kernel device protocol already defined");
     cf_kif = this_proto = proto_config_new(&proto_unix_iface, sizeof(struct kif_config));
     this_proto->preference = DEF_PREF_DIRECT;
     THIS_KIF->scan_time = 60;
     init_list(&THIS_KIF->primary);
     krt_if_construct(THIS_KIF);
   }
 ;


kif_item:
   SCAN TIME expr {
      /* Scan time of 0 means scan on startup only */
      THIS_KIF->scan_time = $3;
   }
 | PRIMARY text_or_none prefix_or_ipa {
     struct kif_primary_item *kpi = cfg_alloc(sizeof (struct kif_primary_item));
     kpi->pattern = $2;
     kpi->prefix = $3.addr;
     kpi->pxlen = $3.len;
     add_tail(&THIS_KIF->primary, &kpi->n);
   }
 ;

/* Grammar from ../../sysdep/linux/netlink/netlink.Y */


nl_item:
   KERNEL TABLE expr {
	if ($3 <= 0 || $3 >= NL_NUM_TABLES)
	  cf_error("Kernel routing table number out of range");
	THIS_KRT->scan.table_id = $3;
   }
 ;

/* Grammar from ../../nest/config.Y */

/* Setting of router ID */


rtrid: ROUTER ID idval ';' {
   new_config->router_id = $3;
   }
 ;

idval:
   NUM { $$ = $1; }
 | RTRID
 | IPA {
#ifndef IPV6
     $$ = ipa_to_u32($1);
#else
     cf_error("Router IDs must be entered as hexadecimal numbers or IPv4 addresses in IPv6 version");
#endif
   }
 ;



listen: LISTEN BGP listen_opts ';' ;

listen_opts:
   /* Nothing */
 | listen_opts listen_opt
 ;

listen_opt: 
   ADDRESS ipa { new_config->listen_bgp_addr = $2; }
 | PORT expr { new_config->listen_bgp_port = $2; }
 | V6ONLY { new_config->listen_bgp_flags |= SKF_V6ONLY; }
 ;


/* Creation of routing tables */


newtab: TABLE SYM {
   rt_new_table($2);
   }
 ;

/* Definition of protocols */


proto_start: PROTOCOL
 ;

proto_name:
   /* EMPTY */ {
     struct symbol *s = cf_default_name(this_proto->protocol->template, &this_proto->protocol->name_counter);
     s->class = SYM_PROTO;
     s->def = this_proto;
     this_proto->name = s->name;
     }
 | SYM {
     cf_define_symbol($1, SYM_PROTO, this_proto);
     this_proto->name = $1->name;
   }
 ;

proto_item:
   /* EMPTY */
 | PREFERENCE expr {
     if ($2 < 0 || $2 > 0xFFFF) cf_error("Invalid preference");
     this_proto->preference = $2;
   }
 | DISABLED bool { this_proto->disabled = $2; }
 | DEBUG debug_mask { this_proto->debug = $2; }
 | MRTDUMP mrtdump_mask { this_proto->mrtdump = $2; }
 | IMPORT imexport { this_proto->in_filter = $2; }
 | EXPORT imexport { this_proto->out_filter = $2; }
 | TABLE rtable { this_proto->table = $2; }
 | ROUTER ID idval { this_proto->router_id = $3; }
 | DESCRIPTION TEXT { this_proto->dsc = $2; }
 ;

imexport:
   FILTER filter { $$ = $2; }
 | where_filter
 | ALL { $$ = FILTER_ACCEPT; }
 | NONE { $$ = FILTER_REJECT; }
 ;

rtable:
   SYM {
     if ($1->class != SYM_TABLE) cf_error("Table name expected");
     $$ = $1->def;
   }
 ;


debug_default:
   DEBUG PROTOCOLS debug_mask { new_config->proto_default_debug = $3; }
 | DEBUG COMMANDS expr { new_config->cli_debug = $3; }
 ;

/* MRTDUMP PROTOCOLS is in systep/unix/config.Y */

/* Interface patterns */

iface_patt_node_init:
   /* EMPTY */ {
     struct iface_patt_node *ipn = cfg_allocz(sizeof(struct iface_patt_node));
     add_tail(&this_ipatt->ipn_list, NODE ipn);
     this_ipn = ipn;
   }
 ;

iface_patt_node_body:
   TEXT { this_ipn->pattern = $1; this_ipn->prefix = IPA_NONE; this_ipn->pxlen = 0; }
 | prefix_or_ipa { this_ipn->pattern = NULL; this_ipn->prefix = $1.addr; this_ipn->pxlen = $1.len; }
 | TEXT prefix_or_ipa { this_ipn->pattern = $1; this_ipn->prefix = $2.addr; this_ipn->pxlen = $2.len; }
 ;

iface_negate:
       { this_ipn->positive = 1; }
 | '-' { this_ipn->positive = 0; }
 ;

iface_patt_node:
   iface_patt_node_init iface_negate iface_patt_node_body 
 ;


iface_patt_list:
   iface_patt_node
 | iface_patt_list ',' iface_patt_node
 ;


/* Direct device route protocol */


dev_proto_start: proto_start DIRECT {
     struct rt_dev_config *p = proto_config_new(&proto_device, sizeof(struct rt_dev_config));
     this_proto = &p->c;
     p->c.preference = DEF_PREF_DIRECT;
     init_list(&p->iface_list);
   }
 ;

dev_proto:
   dev_proto_start proto_name '{'
 | dev_proto proto_item ';'
 | dev_proto dev_iface_patt ';'
 ;

dev_iface_init:
   /* EMPTY */ {
     struct rt_dev_config *p = (void *) this_proto;
     this_ipatt = cfg_allocz(sizeof(struct iface_patt));
     add_tail(&p->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
   }
 ;

dev_iface_patt:
   INTERFACE dev_iface_init iface_patt_list
 ;

/* Debug flags */

debug_mask:
   ALL { $$ = ~0; }
 | OFF { $$ = 0; }
 | '{' debug_list '}' { $$ = $2; }
 ;

debug_list:
   debug_flag
 | debug_list ',' debug_flag { $$ = $1 | $3; }
 ;

debug_flag:
   STATES	{ $$ = D_STATES; }
 | ROUTES	{ $$ = D_ROUTES; }
 | FILTERS	{ $$ = D_FILTERS; }
 | INTERFACES	{ $$ = D_IFACES; }
 | EVENTS	{ $$ = D_EVENTS; }
 | PACKETS	{ $$ = D_PACKETS; }
 ;

/* MRTDump flags */

mrtdump_mask:
   ALL { $$ = ~0; }
 | OFF { $$ = 0; }
 | '{' mrtdump_list '}' { $$ = $2; }
 ;

mrtdump_list:
   mrtdump_flag
 | mrtdump_list ',' mrtdump_flag { $$ = $1 | $3; }
 ;

mrtdump_flag:
   STATES	{ $$ = MD_STATES; }
 | MESSAGES	{ $$ = MD_MESSAGES; }
 ;

/* Password lists */

password_list:
   PASSWORDS '{' password_items '}'
 | password_item
;

password_items: 
    /* empty */
  | password_item ';' password_items
;

password_item:
    password_item_begin '{' password_item_params '}'
  | password_item_begin
;

password_item_begin:
   PASSWORD TEXT {
     if (!this_p_list) {
     	this_p_list = cfg_alloc(sizeof(list));
     	init_list(this_p_list);
        password_id = 1;
     }
     this_p_item = cfg_alloc(sizeof (struct password_item));
     this_p_item->password = $2;
     this_p_item->genfrom = 0;
     this_p_item->gento = TIME_INFINITY;
     this_p_item->accfrom = 0;
     this_p_item->accto = TIME_INFINITY;
     this_p_item->id = password_id++;
     add_tail(this_p_list, &this_p_item->n);
   }
;

password_item_params:
   /* empty */ { } 
 | GENERATE FROM datetime ';' password_item_params { this_p_item->genfrom = $3; }
 | GENERATE TO datetime ';' password_item_params { this_p_item->gento = $3; }
 | ACCEPT FROM datetime ';' password_item_params { this_p_item->accfrom = $3; }
 | ACCEPT TO datetime ';' password_item_params { this_p_item->accto = $3; }
 | ID expr ';' password_item_params { this_p_item->id = $2; if ($2 <= 0) cf_error("Password ID has to be greated than zero."); }
 ;



/* Core commands */


cmd_SHOW_STATUS: SHOW STATUS  END
{ cmd_show_status(); } ;

cmd_SHOW_MEMORY: SHOW MEMORY  END
{ cmd_show_memory(); } ;

cmd_SHOW_PROTOCOLS: SHOW PROTOCOLS proto_patt2 END
{ proto_apply_cmd($3, proto_cmd_show, 0, 0); } ;

cmd_SHOW_PROTOCOLS_ALL: SHOW PROTOCOLS ALL proto_patt2 END
{ proto_apply_cmd($4, proto_cmd_show, 0, 1); } ;

optsym:
   SYM
 | /* empty */ { $$ = NULL; }
 ;

cmd_SHOW_INTERFACES: SHOW INTERFACES  END
{ if_show(); } ;

cmd_SHOW_INTERFACES_SUMMARY: SHOW INTERFACES SUMMARY  END
{ if_show_summary(); } ;

cmd_SHOW_ROUTE: SHOW ROUTE r_args END
{ rt_show($3); } ;

r_args:
   /* empty */ {
     $$ = cfg_allocz(sizeof(struct rt_show_data));
     $$->pxlen = 256;
     $$->filter = FILTER_ACCEPT;
     $$->table = config->master_rtc->table;
   }
 | r_args prefix {
     $$ = $1;
     if ($$->pxlen != 256) cf_error("Only one prefix expected");
     $$->prefix = $2.addr;
     $$->pxlen = $2.len;
   }
 | r_args FOR prefix_or_ipa {
     $$ = $1;
     if ($$->pxlen != 256) cf_error("Only one prefix expected");
     $$->prefix = $3.addr;
     $$->pxlen = $3.len;
     $$->show_for = 1;
   }
 | r_args TABLE SYM {
     $$ = $1;
     if ($3->class != SYM_TABLE) cf_error("%s is not a table", $3->name);
     $$->table = ((struct rtable_config *)$3->def)->table;
   }
 | r_args FILTER filter {
     $$ = $1;
     if ($$->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     $$->filter = $3;
   }
 | r_args where_filter {
     $$ = $1;
     if ($$->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     $$->filter = $2;
   }
 | r_args ALL {
     $$ = $1;
     $$->verbose = 1;
   }
 | r_args PRIMARY {
     $$ = $1;
     $$->primary_only = 1;
   }
 | r_args export_or_preexport SYM {
     struct proto_config *c = (struct proto_config *) $3->def;
     $$ = $1;
     if ($$->export_mode) cf_error("Protocol specified twice");
     if ($3->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", $3->name);
     $$->export_mode = $2;
     $$->primary_only = 1;
     $$->export_protocol = c->proto;
     $$->running_on_config = c->proto->cf->global;
   }
 | r_args PROTOCOL SYM {
     struct proto_config *c = (struct proto_config *) $3->def;
     $$ = $1;
     if ($$->show_protocol) cf_error("Protocol specified twice");
     if ($3->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", $3->name);
     $$->show_protocol = c->proto;
     $$->running_on_config = c->proto->cf->global;
   }
 | r_args STATS {
     $$ = $1;
     $$->stats = 1;
   }
 | r_args COUNT {
     $$ = $1;
     $$->stats = 2;
   }
 ;

export_or_preexport:
   PREEXPORT { $$ = 1; }
 | EXPORT { $$ = 2; }
 ;

cmd_SHOW_SYMBOLS: SHOW SYMBOLS optsym END
{ cmd_show_symbols($3); } ;


cmd_DUMP_RESOURCES: DUMP RESOURCES  END
{ rdump(&root_pool); cli_msg(0, ""); } ;
cmd_DUMP_SOCKETS: DUMP SOCKETS  END
{ sk_dump_all(); cli_msg(0, ""); } ;
cmd_DUMP_INTERFACES: DUMP INTERFACES  END
{ if_dump_all(); cli_msg(0, ""); } ;
cmd_DUMP_NEIGHBORS: DUMP NEIGHBORS  END
{ neigh_dump_all(); cli_msg(0, ""); } ;
cmd_DUMP_ATTRIBUTES: DUMP ATTRIBUTES  END
{ rta_dump_all(); cli_msg(0, ""); } ;
cmd_DUMP_ROUTES: DUMP ROUTES  END
{ rt_dump_all(); cli_msg(0, ""); } ;
cmd_DUMP_PROTOCOLS: DUMP PROTOCOLS  END
{ protos_dump_all(); cli_msg(0, ""); } ;

cmd_ECHO: ECHO echo_mask echo_size END {
  cli_set_log_echo(this_cli, $2, $3);
  cli_msg(0, "");
} ;

echo_mask:
   ALL { $$ = ~0; }
 | OFF { $$ = 0; }
 | NUM
 ;

echo_size:
   /* empty */ { $$ = 4096; }
 | NUM {
     if ($1 < 256 || $1 > 65536) cf_error("Invalid log buffer size");
     $$ = $1;
   }
 ;

cmd_DISABLE: DISABLE proto_patt END
{ proto_apply_cmd($2, proto_cmd_disable, 1, 0); } ;
cmd_ENABLE: ENABLE proto_patt END
{ proto_apply_cmd($2, proto_cmd_enable, 1, 0); } ;
cmd_RESTART: RESTART proto_patt END
{ proto_apply_cmd($2, proto_cmd_restart, 1, 0); } ;
cmd_RELOAD: RELOAD proto_patt END
{ proto_apply_cmd($2, proto_cmd_reload, 1, CMD_RELOAD); } ;
cmd_RELOAD_IN: RELOAD IN proto_patt END
{ proto_apply_cmd($3, proto_cmd_reload, 1, CMD_RELOAD_IN); } ;
cmd_RELOAD_OUT: RELOAD OUT proto_patt END
{ proto_apply_cmd($3, proto_cmd_reload, 1, CMD_RELOAD_OUT); } ;


cmd_DEBUG: DEBUG proto_patt debug_mask END
{ proto_apply_cmd($2, proto_cmd_debug, 1, $3); } ;


cmd_MRTDUMP: MRTDUMP proto_patt mrtdump_mask END
{ proto_apply_cmd($2, proto_cmd_mrtdump, 1, $3); } ;

cmd_RESTRICT: RESTRICT  END
{ this_cli->restricted = 1; cli_msg(16, "Access restricted"); } ;

proto_patt:
   SYM  { $$.ptr = $1; $$.patt = 0; }
 | ALL  { $$.ptr = NULL; $$.patt = 1; }
 | TEXT { $$.ptr = $1; $$.patt = 1; }
 ;

proto_patt2:
   SYM  { $$.ptr = $1; $$.patt = 0; }
 |      { $$.ptr = NULL; $$.patt = 1; }
 | TEXT { $$.ptr = $1; $$.patt = 1; }
 ;


/* Grammar from ../../filter/config.Y */

filter_def:
   FILTER SYM { $2 = cf_define_symbol($2, SYM_FILTER, NULL); cf_push_scope( $2 ); }
     filter_body {
     $2->def = $4;
     $4->name = $2->name;
     DBG( "We have new filter defined (%s)\n", $2->name );
     cf_pop_scope();
   }
 ;

filter_eval:
   EVAL term { f_eval_int($2); }
 ;

type:
   INT { $$ = T_INT; }
 | BOOL { $$ = T_BOOL; }
 | IP { $$ = T_IP; }
 | PREFIX { $$ = T_PREFIX; }
 | PAIR { $$ = T_PAIR; }
 | QUAD { $$ = T_QUAD; }
 | STRING { $$ = T_STRING; }
 | BGPMASK { $$ = T_PATH_MASK; }
 | BGPPATH { $$ = T_PATH; }
 | CLIST { $$ = T_CLIST; }
 | type SET { 
	switch ($1) {
	  case T_INT:
	  case T_PAIR:
	  case T_QUAD:
	  case T_IP:
	       $$ = T_SET;
	       break;

	  case T_PREFIX:
	       $$ = T_PREFIX_SET;
	    break;

	  default:
		cf_error( "You can't create sets of this type." );
	}
   }
 ;

one_decl:
   type SYM {
     struct f_val * val = cfg_alloc(sizeof(struct f_val)); 
     val->type = $1; 
     $2 = cf_define_symbol($2, SYM_VARIABLE | $1, val);
     DBG( "New variable %s type %x\n", $2->name, $1 );
     $2->aux2 = NULL;
     $$=$2;
   }
 ;

/* Decls with ';' at the end */
decls: /* EMPTY */ { $$ = NULL; }
 | one_decl ';' decls {
     $$ = $1;
     $$->aux2 = $3;
   }
 ;

/* Declarations that have no ';' at the end. */
declsn: one_decl { $$ = $1; }
 | declsn ';' one_decl {
     $$ = $1;
     $$->aux2 = $3;
   }
 ;

filter_body:
   function_body {
     struct filter *f = cfg_alloc(sizeof(struct filter));
     f->name = NULL;
     f->root = $1;
     $$ = f;
   }
 ;

filter:
   SYM {
     if ($1->class != SYM_FILTER) cf_error("No such filter.");
     $$ = $1->def;
   }
 | filter_body
 ;

where_filter:
   WHERE term {
     /* Construct 'IF term THEN ACCEPT; REJECT;' */
     struct filter *f = cfg_alloc(sizeof(struct filter));
     struct f_inst *i, *acc, *rej;
     acc = f_new_inst();		/* ACCEPT */
     acc->code = P('p',',');
     acc->a1.p = NULL;
     acc->a2.i = F_ACCEPT;
     rej = f_new_inst();		/* REJECT */
     rej->code = P('p',',');
     rej->a1.p = NULL;
     rej->a2.i = F_REJECT;
     i = f_new_inst();			/* IF */
     i->code = '?';
     i->a1.p = $2;
     i->a2.p = acc;
     i->next = rej;
     f->name = NULL;
     f->root = i;
     $$ = f;
  }
 ;

function_params:
   '(' declsn ')' { DBG( "Have function parameters\n" ); $$=$2; }
 | '(' ')' { $$=NULL; }
 ;

function_body:
   decls '{' cmds '}' {
     if ($1) {
       /* Prepend instruction to clear local variables */
       $$ = f_new_inst();
       $$->code = P('c','v');
       $$->a1.p = $1;
       $$->next = $3;
     } else
       $$ = $3;
   }
 ;

function_def:
   FUNCTION SYM { DBG( "Beginning of function %s\n", $2->name );
     $2 = cf_define_symbol($2, SYM_FUNCTION, NULL);
     cf_push_scope($2);
   } function_params function_body {
     $2->def = $5;
     $2->aux2 = $4;
     DBG("Hmm, we've got one function here - %s\n", $2->name); 
     cf_pop_scope();
   }
 ;

/* Programs */

/* Hack: $$ of cmds_int is the last node.
   $$->next of cmds_int is temporary used for the first node */

cmds: /* EMPTY */ { $$ = NULL; }
 | cmds_int { $$ = $1->next; $1->next = NULL; }
 ;

cmds_int: cmd { $$ = $1; $1->next = $1; }
 | cmds_int cmd { $$ = $2; $2->next = $1->next ; $1->next = $2; }
 ;

block:
   cmd {
     $$=$1;
   }
 | '{' cmds '}' {
     $$=$2;
   }
 ;

/*
 * Simple types, their bison value is int
 */
cpair:
   '(' NUM ',' NUM ')' { $$ = make_pair($2, $4); }
 ;

/*
 * Complex types, their bison value is struct f_val
 */
fipa:
   IPA %prec PREFIX_DUMMY { $$.type = T_IP; $$.val.px.ip = $1; }
 ;

set_atom:
   NUM   { $$.type = T_INT; $$.val.i = $1; }
 | RTRID { $$.type = T_QUAD; $$.val.i = $1; }
 | cpair { $$.type = T_PAIR; $$.val.i = $1; }
 | fipa  { $$ = $1; }
 | ENUM  {  $$.type = $1 >> 16; $$.val.i = $1 & 0xffff; }
 ; 

set_item:
   '(' NUM ',' '*' ')' { 
	$$ = f_new_tree(); 
	$$->from.type = $$->to.type = T_PAIR;
	$$->from.val.i = make_pair($2, 0); 
	$$->to.val.i = make_pair($2, 0xffff);
   }
 | set_atom { 
	$$ = f_new_tree(); 
	$$->from = $1; 
	$$->to = $1;
   }
 | set_atom '.' '.' set_atom { 
	$$ = f_new_tree(); 
	$$->from = $1; 
	$$->to = $4; 
   }
 ;

set_items:
   set_item { $$ = $1; }
 | set_items ',' set_item { $$ = $3; $$->left = $1; }
 ;

fprefix_s:
   IPA '/' NUM %prec '/' {
     if (($3 < 0) || ($3 > MAX_PREFIX_LENGTH) || !ip_is_prefix($1, $3)) cf_error("Invalid network prefix: %I/%d.", $1, $3);
     $$.type = T_PREFIX; $$.val.px.ip = $1; $$.val.px.len = $3;
   }
 ;

fprefix:
   fprefix_s { $$ = $1; }
 | fprefix_s '+' { $$ = $1; $$.val.px.len |= LEN_PLUS; }
 | fprefix_s '-' { $$ = $1; $$.val.px.len |= LEN_MINUS; }
 | fprefix_s '{' NUM ',' NUM '}' { 
     if (! ((0 <= $3) && ($3 <= $5) && ($5 <= MAX_PREFIX_LENGTH))) cf_error("Invalid prefix pattern range: {%d, %d}.", $3, $5);
     $$ = $1; $$.val.px.len |= LEN_RANGE | ($3 << 16) | ($5 << 8);
   }
 ;

fprefix_set:
   fprefix { $$ = f_new_trie(); trie_add_prefix($$, &($1.val.px)); }
 | fprefix_set ',' fprefix { $$ = $1; trie_add_prefix($$, &($3.val.px)); }
 ;

switch_body: /* EMPTY */ { $$ = NULL; }
 | switch_body set_item ':' cmds  {
     $$ = $2;
     $$->data = $4;
     $$->left = $1;
   }
 | switch_body ELSE ':' cmds {
     $$ = f_new_tree(); 
     $$->from.type = T_VOID; 
     $$->to.type = T_VOID;
     $$->data = $4;
     $$->left = $1;
   }
 ;

/* CONST '(' expr ')' { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_INT; $$->a2.i = $3; } */

bgp_path_expr:
   symbol       { $$ = $1; }   
 | '(' term ')' { $$ = $2; }
 ;

bgp_path:
   PO  bgp_path_tail1 PC  { $$ = $2; }
 | '/' bgp_path_tail2 '/' { $$ = $2; }
 ;

bgp_path_tail1:
   NUM bgp_path_tail1 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_ASN;      $$->val = $1; }
 | '*' bgp_path_tail1 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_ASTERISK; $$->val  = 0; }
 | '?' bgp_path_tail1 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_QUESTION; $$->val  = 0; }
 | bgp_path_expr bgp_path_tail1 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_ASN_EXPR; $$->val = (uintptr_t) $1; }
 |  		      { $$ = NULL; }
 ;

bgp_path_tail2:
   NUM bgp_path_tail2 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_ASN;      $$->val = $1; }
 | '?' bgp_path_tail2 { $$ = cfg_alloc(sizeof(struct f_path_mask)); $$->next = $2; $$->kind = PM_ASTERISK; $$->val  = 0; }
 | 		      { $$ = NULL; }
 ;

dpair:
   '(' term ',' term ')' {
        if (($2->code == 'c') && ($4->code == 'c'))
          { 
            if (($2->aux != T_INT) || ($4->aux != T_INT))
              cf_error( "Can't operate with value of non-integer type in pair constructor" );
            $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_PAIR;  $$->a2.i = make_pair($2->a2.i, $4->a2.i);
          }
	else
	  { $$ = f_new_inst(); $$->code = P('m', 'p'); $$->a1.p = $2; $$->a2.p = $4; }
    }
 ;

constant:
   NUM    { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_INT;  $$->a2.i = $1; }
 | TRUE   { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_BOOL; $$->a2.i = 1;  }
 | FALSE  { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_BOOL; $$->a2.i = 0;  }
 | TEXT   { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_STRING; $$->a2.p = $1; }
 | fipa	   { NEW_F_VAL; $$ = f_new_inst(); $$->code = 'C'; $$->a1.p = val; *val = $1; }
 | fprefix_s {NEW_F_VAL; $$ = f_new_inst(); $$->code = 'C'; $$->a1.p = val; *val = $1; }
 | RTRID  { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_QUAD;  $$->a2.i = $1; }
 | '[' set_items ']' { DBG( "We've got a set here..." ); $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_SET; $$->a2.p = build_tree($2); DBG( "ook\n" ); }
 | '[' fprefix_set ']' { $$ = f_new_inst(); $$->code = 'c'; $$->aux = T_PREFIX_SET;  $$->a2.p = $2; }
 | ENUM	  { $$ = f_new_inst(); $$->code = 'c'; $$->aux = $1 >> 16; $$->a2.i = $1 & 0xffff; }
 | bgp_path { NEW_F_VAL; $$ = f_new_inst(); $$->code = 'C'; val->type = T_PATH_MASK; val->val.path_mask = $1; $$->a1.p = val; }
 ;


/*
 *  Maybe there are no dynamic attributes defined by protocols.
 *  For such cases, we force the dynamic_attr list to contain
 *  at least an invalid token, so it's syntantically correct.
 */

rtadot: /* EMPTY, we are not permitted RTA. prefix */
 ;

function_call:
   SYM '(' var_list ')' {
     struct symbol *sym;
     struct f_inst *inst = $3;
     if ($1->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", $1->name);
     $$ = f_new_inst();
     $$->code = P('c','a');
     $$->a1.p = inst;
     $$->a2.p = $1->def;
     sym = $1->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", $1->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
 ;

symbol:
   SYM {
     $$ = f_new_inst();
     switch ($1->class) {
       case SYM_NUMBER:
	$$ = f_new_inst();
	$$->code = 'c'; 
	$$->aux = T_INT; 
	$$->a2.i = $1->aux;
	break;
       case SYM_IPA:
	{ NEW_F_VAL; $$ = f_new_inst(); $$->code = 'C'; $$->a1.p = val; val->type = T_IP; val->val.px.ip = * (ip_addr *) ($1->def); }
	break;
       case SYM_VARIABLE | T_BOOL:
       case SYM_VARIABLE | T_INT:
       case SYM_VARIABLE | T_PAIR:
       case SYM_VARIABLE | T_QUAD:
       case SYM_VARIABLE | T_STRING:
       case SYM_VARIABLE | T_IP:
       case SYM_VARIABLE | T_PREFIX:
       case SYM_VARIABLE | T_PREFIX_SET:
       case SYM_VARIABLE | T_SET:
       case SYM_VARIABLE | T_PATH:
       case SYM_VARIABLE | T_PATH_MASK:
       case SYM_VARIABLE | T_CLIST:
	 $$->code = 'V';
	 $$->a1.p = $1->def;
	 $$->a2.p = $1->name;
	 break;
       default:
	 cf_error("%s: variable expected.", $1->name );
     }
   }

static_attr:
   FROM    { $$ = f_new_inst(); $$->aux = T_IP;         $$->a2.i = OFFSETOF(struct rta, from);   $$->a1.i = 1; }

 | GW      { $$ = f_new_inst(); $$->aux = T_IP;         $$->a2.i = OFFSETOF(struct rta, gw);     $$->a1.i = 1; }
 | NET     { $$ = f_new_inst(); $$->aux = T_PREFIX;     $$->a2.i = 0x12345678; /* This is actually ok - T_PREFIX is special-cased. */ }
 | PROTO   { $$ = f_new_inst(); $$->aux = T_STRING;     $$->a2.i = 0x12345678; /* T_STRING is also special-cased. */ }
 | SOURCE  { $$ = f_new_inst(); $$->aux = T_ENUM_RTS;   $$->a2.i = OFFSETOF(struct rta, source); }
 | SCOPE   { $$ = f_new_inst(); $$->aux = T_ENUM_SCOPE; $$->a2.i = OFFSETOF(struct rta, scope);  $$->a1.i = 1; }
 | CAST    { $$ = f_new_inst(); $$->aux = T_ENUM_RTC;   $$->a2.i = OFFSETOF(struct rta, cast); }
 | DEST    { $$ = f_new_inst(); $$->aux = T_ENUM_RTD;   $$->a2.i = OFFSETOF(struct rta, dest); }
 ;

term:
   '(' term ')'      { $$ = $2; }
 | term '+' term     { $$ = f_new_inst(); $$->code = '+';        $$->a1.p = $1; $$->a2.p = $3; }
 | term '-' term     { $$ = f_new_inst(); $$->code = '-';        $$->a1.p = $1; $$->a2.p = $3; }
 | term '*' term     { $$ = f_new_inst(); $$->code = '*';        $$->a1.p = $1; $$->a2.p = $3; }
 | term '/' term     { $$ = f_new_inst(); $$->code = '/';        $$->a1.p = $1; $$->a2.p = $3; }
 | term AND term     { $$ = f_new_inst(); $$->code = '&';        $$->a1.p = $1; $$->a2.p = $3; }
 | term OR  term     { $$ = f_new_inst(); $$->code = '|';        $$->a1.p = $1; $$->a2.p = $3; }
 | term '=' term     { $$ = f_new_inst(); $$->code = P('=','='); $$->a1.p = $1; $$->a2.p = $3; }
 | term NEQ term { $$ = f_new_inst(); $$->code = P('!','=');     $$->a1.p = $1; $$->a2.p = $3; }
 | term '<' term     { $$ = f_new_inst(); $$->code = '<';        $$->a1.p = $1; $$->a2.p = $3; }
 | term LEQ term { $$ = f_new_inst(); $$->code = P('<','=');     $$->a1.p = $1; $$->a2.p = $3; }
 | term '>' term     { $$ = f_new_inst(); $$->code = '<';        $$->a1.p = $3; $$->a2.p = $1; }
 | term GEQ term { $$ = f_new_inst(); $$->code = P('<','=');     $$->a1.p = $3; $$->a2.p = $1; }
 | term '~' term     { $$ = f_new_inst(); $$->code = '~';        $$->a1.p = $1; $$->a2.p = $3; }
 | '!' term { $$ = f_new_inst(); $$->code = '!'; $$->a1.p = $2; }
 | DEFINED '(' term ')' { $$ = f_new_inst(); $$->code = P('d','e');  $$->a1.p = $3; }

 | symbol   { $$ = $1; }
 | constant { $$ = $1; }
 | dpair    { $$ = $1; }

 | PREFERENCE { $$ = f_new_inst(); $$->code = 'P'; }

 | rtadot static_attr { $$ = $2; $$->code = 'a'; }

 | rtadot dynamic_attr { $$ = $2; $$->code = P('e','a'); }

 | term '.' IP { $$ = f_new_inst(); $$->code = P('c','p'); $$->a1.p = $1; $$->aux = T_IP; }
 | term '.' LEN { $$ = f_new_inst(); $$->code = 'L'; $$->a1.p = $1; }
 | term '.' MASK '(' term ')' { $$ = f_new_inst(); $$->code = P('i','M'); $$->a1.p = $1; $$->a2.p = $5; }
 | term '.' FIRST { $$ = f_new_inst(); $$->code = P('a','f'); $$->a1.p = $1; }
 | term '.' LAST  { $$ = f_new_inst(); $$->code = P('a','l'); $$->a1.p = $1; }

/* Communities */
/* This causes one shift/reduce conflict
 | rtadot dynamic_attr '.' ADD '(' term ')' { }
 | rtadot dynamic_attr '.' DELETE '(' term ')' { }
 | rtadot dynamic_attr '.' CONTAINS '(' term ')' { }
 | rtadot dynamic_attr '.' RESET{ }
*/

 | '+' EMPTY '+' { $$ = f_new_inst(); $$->code = 'E'; $$->aux = T_PATH; }
 | '-' EMPTY '-' { $$ = f_new_inst(); $$->code = 'E'; $$->aux = T_CLIST; }
 | PREPEND '(' term ',' term ')' { $$ = f_new_inst(); $$->code = P('A','p'); $$->a1.p = $3; $$->a2.p = $5; } 
 | ADD '(' term ',' term ')' { $$ = f_new_inst(); $$->code = P('C','a'); $$->a1.p = $3; $$->a2.p = $5; $$->aux = 'a'; } 
 | DELETE '(' term ',' term ')' { $$ = f_new_inst(); $$->code = P('C','a'); $$->a1.p = $3; $$->a2.p = $5; $$->aux = 'd'; }

/* | term '.' LEN { $$->code = P('P','l'); } */

/* function_call is inlined here */
 | SYM '(' var_list ')' {
     struct symbol *sym;
     struct f_inst *inst = $3;
     if ($1->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", $1->name);
     $$ = f_new_inst();
     $$->code = P('c','a');
     $$->a1.p = inst;
     $$->a2.p = $1->def;
     sym = $1->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", $1->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
 ;

break_command:
   QUITBIRD { $$ = F_QUITBIRD; }
 | ACCEPT { $$ = F_ACCEPT; }
 | REJECT { $$ = F_REJECT; }
 | ERROR { $$ = F_ERROR; }
 | PRINT { $$ = F_NOP; }
 | PRINTN { $$ = F_NONL; }
 ;

print_one:
   term { $$ = f_new_inst(); $$->code = 'p'; $$->a1.p = $1; $$->a2.p = NULL; }
 ;

print_list: /* EMPTY */ { $$ = NULL; }
 | print_one { $$ = $1; }
 | print_one ',' print_list {
     if ($1) {
       $1->next = $3;
       $$ = $1;
     } else $$ = $3;
   }
 
 ;

var_listn: term { 
     $$ = f_new_inst();
     $$->code = 's';
     $$->a1.p = NULL;
     $$->a2.p = $1;
     $$->next = NULL;
   }
 | term ',' var_listn {
     $$ = f_new_inst();
     $$->code = 's';
     $$->a1.p = NULL;
     $$->a2.p = $1;
     $$->next = $3;
   }
 ;

var_list: /* EMPTY */ { $$ = NULL; }
 | var_listn { $$ = $1; }
 ;

cmd:
   IF term THEN block {
     $$ = f_new_inst();
     $$->code = '?';
     $$->a1.p = $2;
     $$->a2.p = $4;
   }
 | IF term THEN block ELSE block {
     struct f_inst *i = f_new_inst();
     i->code = '?';
     i->a1.p = $2;
     i->a2.p = $4;
     $$ = f_new_inst();
     $$->code = '?';
     $$->a1.p = i;
     $$->a2.p = $6;
   }
 | SYM '=' term ';' {
     $$ = f_new_inst();
     DBG( "Ook, we'll set value\n" );
     if (($1->class & ~T_MASK) != SYM_VARIABLE)
       cf_error( "You may set only variables." );
     $$->code = 's';
     $$->a1.p = $1;
     $$->a2.p = $3;
   }
 | RETURN term ';' {
     $$ = f_new_inst();
     DBG( "Ook, we'll return the value\n" );
     $$->code = 'r';
     $$->a1.p = $2;
   }
 | rtadot dynamic_attr '=' term ';' {
     $$ = $2;
     $$->code = P('e','S');
     $$->a1.p = $4;
   }
 | rtadot static_attr '=' term ';' {
     $$ = $2;
     if (!$$->a1.i)
       cf_error( "This static attribute is read-only.");
     $$->code = P('a','S');
     $$->a1.p = $4;
   }
 | PREFERENCE '=' term ';' {
     $$ = f_new_inst();
     $$->code = P('P','S');
     $$->a1.p = $3;
   } 
 | UNSET '(' rtadot dynamic_attr ')' ';' {
     $$ = $4;
     $$->aux = EAF_TYPE_UNDEF | EAF_TEMP;
     $$->code = P('e','S');
     $$->a1.p = NULL;
   }
 | break_command print_list ';' { $$ = f_new_inst(); $$->code = P('p',','); $$->a1.p = $2; $$->a2.i = $1; }
 | function_call ';' { $$ = $1; }
 | CASE term '{' switch_body '}' {
      $$ = f_new_inst();
      $$->code = P('S','W');
      $$->a1.p = $2;
      $$->a2.p = build_tree( $4 );
   }


 | rtadot dynamic_attr '.' EMPTY ';' 
  { struct f_inst *i = f_new_inst(); i->code = 'E'; i->aux = T_CLIST; $$ = $2; $$->code = P('e','S'); $$->a1.p = i; }
 | rtadot dynamic_attr '.' PREPEND '(' term ')' ';'   { $$ = f_generate_complex( P('A','p'), 'x', $2, $6 ); }
 | rtadot dynamic_attr '.' ADD '(' term ')' ';'       { $$ = f_generate_complex( P('C','a'), 'a', $2, $6 ); } 
 | rtadot dynamic_attr '.' DELETE '(' term ')' ';'    { $$ = f_generate_complex( P('C','a'), 'd', $2, $6 ); } 
 ;

/* Grammar from ../../proto/bgp/config.Y */


bgp_proto_start: proto_start BGP {
     this_proto = proto_config_new(&proto_bgp, sizeof(struct bgp_config));
     this_proto->preference = DEF_PREF_BGP;
     BGP_CFG->hold_time = 240;
     BGP_CFG->connect_retry_time = 120;
     BGP_CFG->initial_hold_time = 240;
     BGP_CFG->compare_path_lengths = 1;
     BGP_CFG->start_delay_time = 5;
     BGP_CFG->error_amnesia_time = 300;
     BGP_CFG->error_delay_time_min = 60;
     BGP_CFG->error_delay_time_max = 300;
     BGP_CFG->enable_refresh = 1;
     BGP_CFG->enable_as4 = 1;
     BGP_CFG->capabilities = 2;
     BGP_CFG->advertise_ipv4 = 1;
     BGP_CFG->interpret_communities = 1;
     BGP_CFG->default_local_pref = 100;
 }
 ;

bgp_proto:
   bgp_proto_start proto_name '{'
 | bgp_proto proto_item ';'
 | bgp_proto LOCAL AS expr ';' { BGP_CFG->local_as = $4; }
 | bgp_proto NEIGHBOR ipa AS expr ';' {
     if (ipa_nonzero(BGP_CFG->remote_ip)) cf_error("Only one neighbor per BGP instance is allowed");

     BGP_CFG->remote_ip = $3;
     BGP_CFG->remote_as = $5;
   }
 | bgp_proto RR CLUSTER ID idval ';' { BGP_CFG->rr_cluster_id = $5; }
 | bgp_proto RR CLIENT ';' { BGP_CFG->rr_client = 1; }
 | bgp_proto RS CLIENT ';' { BGP_CFG->rs_client = 1; }
 | bgp_proto HOLD TIME expr ';' { BGP_CFG->hold_time = $4; }
 | bgp_proto STARTUP HOLD TIME expr ';' { BGP_CFG->initial_hold_time = $5; }
 | bgp_proto CONNECT RETRY TIME expr ';' { BGP_CFG->connect_retry_time = $5; }
 | bgp_proto KEEPALIVE TIME expr ';' { BGP_CFG->keepalive_time = $4; }
 | bgp_proto MULTIHOP expr VIA ipa ';' { BGP_CFG->multihop = $3; BGP_CFG->multihop_via = $5; }
 | bgp_proto NEXT HOP SELF ';' { BGP_CFG->next_hop_self = 1; }
 | bgp_proto MISSING LLADDR SELF ';' { BGP_CFG->missing_lladdr = MLL_SELF; }
 | bgp_proto MISSING LLADDR DROP ';' { BGP_CFG->missing_lladdr = MLL_DROP; }
 | bgp_proto MISSING LLADDR IGNORE ';' { BGP_CFG->missing_lladdr = MLL_IGNORE; }
 | bgp_proto PATH METRIC bool ';' { BGP_CFG->compare_path_lengths = $4; }
 | bgp_proto PREFER OLDER bool ';' { BGP_CFG->prefer_older = $4; }
 | bgp_proto DEFAULT BGP_MED expr ';' { BGP_CFG->default_med = $4; }
 | bgp_proto DEFAULT BGP_LOCAL_PREF expr ';' { BGP_CFG->default_local_pref = $4; }
 | bgp_proto SOURCE ADDRESS ipa ';' { BGP_CFG->source_addr = $4; }
 | bgp_proto START DELAY TIME expr ';' { BGP_CFG->start_delay_time = $5; }
 | bgp_proto ERROR FORGET TIME expr ';' { BGP_CFG->error_amnesia_time = $5; } 
 | bgp_proto ERROR WAIT TIME expr ',' expr ';' { BGP_CFG->error_delay_time_min = $5; BGP_CFG->error_delay_time_max = $7; }
 | bgp_proto DISABLE AFTER ERROR bool ';' { BGP_CFG->disable_after_error = $5; }
 | bgp_proto ENABLE ROUTE REFRESH bool ';' { BGP_CFG->enable_refresh = $5; }
 | bgp_proto ENABLE AS4 bool ';' { BGP_CFG->enable_as4 = $4; }
 | bgp_proto CAPABILITIES bool ';' { BGP_CFG->capabilities = $3; }
 | bgp_proto ADVERTISE IPV4 bool ';' { BGP_CFG->advertise_ipv4 = $4; }
 | bgp_proto PASSWORD TEXT ';' { BGP_CFG->password = $3; }
 | bgp_proto ROUTE LIMIT expr ';' { BGP_CFG->route_limit = $4; }
 | bgp_proto PASSIVE bool ';' { BGP_CFG->passive = $3; }
 | bgp_proto INTERPRET COMMUNITIES bool ';' { BGP_CFG->interpret_communities = $4; }
 ;





/* Grammar from ../../proto/ospf/config.Y */


ospf_proto_start: proto_start OSPF {
     this_proto = proto_config_new(&proto_ospf, sizeof(struct ospf_config));
     this_proto->preference = DEF_PREF_OSPF;
     init_list(&OSPF_CFG->area_list);
     OSPF_CFG->rfc1583 = DEFAULT_RFC1583;
     OSPF_CFG->tick = DEFAULT_OSPFTICK;
  }
 ;

ospf_proto:
   ospf_proto_start proto_name '{'
 | ospf_proto ospf_proto_item ';'
 ;

ospf_proto_item:
   proto_item
 | RFC1583COMPAT bool { OSPF_CFG->rfc1583 = $2; }
 | TICK expr { OSPF_CFG->tick = $2 ; if($2<=0) cf_error("Tick must be greater than zero"); }
 | ospf_area '}'
 ;

ospf_area_start: AREA idval '{' {
  this_area = cfg_allocz(sizeof(struct ospf_area_config));
  add_tail(&OSPF_CFG->area_list, NODE this_area);
  this_area->areaid = $2;
  this_area->stub = 0;
  init_list(&this_area->patt_list);
  init_list(&this_area->vlink_list);
  init_list(&this_area->net_list);
  init_list(&this_area->stubnet_list);
 }
 ;

ospf_area: ospf_area_start ospf_area_opts
 ;

ospf_area_opts:
   /* empty */
 | ospf_area_opts ospf_area_item ';'
 ;

ospf_area_item:
   STUB COST expr { this_area->stub = $3 ; if($3<=0) cf_error("Stub cost must be greater than zero"); }
 | STUB bool {if($2) { if(!this_area->stub) this_area->stub=DEFAULT_STUB_COST;}else{ this_area->stub=0;}}
 | NETWORKS '{' pref_list '}'
 | STUBNET ospf_stubnet
 | INTERFACE ospf_iface
 | ospf_vlink
 ;

ospf_stubnet:
   ospf_stubnet_start '{' ospf_stubnet_opts '}'
 | ospf_stubnet_start
 ;

ospf_stubnet_start:
   prefix {
     this_stubnet = cfg_allocz(sizeof(struct ospf_stubnet_config));
     add_tail(&this_area->stubnet_list, NODE this_stubnet);
     this_stubnet->px = $1;
     this_stubnet->cost = COST_D;
   } 
 ;

ospf_stubnet_opts:
   /* empty */
 | ospf_stubnet_opts ospf_stubnet_item ';'
 ;

ospf_stubnet_item:
   HIDDEN bool { this_stubnet->hidden = $2; }
 | SUMMARY bool { this_stubnet->summary = $2; }
 | COST expr { this_stubnet->cost = $2; }
 ;

ospf_vlink:
   ospf_vlink_start '{' ospf_vlink_opts '}' { finish_iface_config(OSPF_PATT); }
 | ospf_vlink_start
 ;

ospf_vlink_opts:
   /* empty */
 | ospf_vlink_opts ospf_vlink_item ';'
 ;

ospf_vlink_item:
 | HELLO expr { OSPF_PATT->helloint = $2 ; if (($2<=0) || ($2>65535)) cf_error("Hello interval must be in range 1-65535"); }
 | RETRANSMIT expr { OSPF_PATT->rxmtint = $2 ; if ($2<=0) cf_error("Retransmit int must be greater than zero"); }
 | TRANSMIT DELAY expr { OSPF_PATT->inftransdelay = $3 ; if (($3<=0) || ($3>65535)) cf_error("Transmit delay must be in range 1-65535"); }
 | WAIT expr { OSPF_PATT->waitint = $2 ; }
 | DEAD expr { OSPF_PATT->dead = $2 ; if ($2<=1) cf_error("Dead interval must be greater than one"); }
 | DEAD COUNT expr { OSPF_PATT->deadc = $3 ; if ($3<=1) cf_error("Dead count must be greater than one"); }
 | AUTHENTICATION NONE { OSPF_PATT->autype = OSPF_AUTH_NONE ; }
 | AUTHENTICATION SIMPLE { OSPF_PATT->autype = OSPF_AUTH_SIMPLE ; }
 | AUTHENTICATION CRYPTOGRAPHIC { OSPF_PATT->autype = OSPF_AUTH_CRYPT ; }
 | password_list 
 ;

ospf_vlink_start: VIRTUAL LINK idval
 {
  if (this_area->areaid == 0) cf_error("Virtual link cannot be in backbone");
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&this_area->vlink_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->vid = $3;
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->waitint = WAIT_DMH*HELLOINT_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->dead = 0;
  OSPF_PATT->type = OSPF_IT_VLINK;
  init_list(&OSPF_PATT->nbma_list);
  OSPF_PATT->autype = OSPF_AUTH_NONE;
  reset_passwords();
 }
;

ospf_iface_item:
   COST expr { OSPF_PATT->cost = $2 ; if (($2<=0) || ($2>65535)) cf_error("Cost must be in range 1-65535"); }
 | HELLO expr { OSPF_PATT->helloint = $2 ; if (($2<=0) || ($2>65535)) cf_error("Hello interval must be in range 1-65535"); }
 | POLL expr { OSPF_PATT->pollint = $2 ; if ($2<=0) cf_error("Poll int must be greater than zero"); }
 | RETRANSMIT expr { OSPF_PATT->rxmtint = $2 ; if ($2<=0) cf_error("Retransmit int must be greater than zero"); }
 | TRANSMIT DELAY expr { OSPF_PATT->inftransdelay = $3 ; if (($3<=0) || ($3>65535)) cf_error("Transmit delay must be in range 1-65535"); }
 | PRIORITY expr { OSPF_PATT->priority = $2 ; if (($2<0) || ($2>255)) cf_error("Priority must be in range 0-255"); }
 | WAIT expr { OSPF_PATT->waitint = $2 ; }
 | DEAD expr { OSPF_PATT->dead = $2 ; if ($2<=1) cf_error("Dead interval must be greater than one"); }
 | DEAD COUNT expr { OSPF_PATT->deadc = $3 ; if ($3<=1) cf_error("Dead count must be greater than one"); }
 | TYPE BROADCAST { OSPF_PATT->type = OSPF_IT_BCAST ; }
 | TYPE NONBROADCAST { OSPF_PATT->type = OSPF_IT_NBMA ; }
 | TYPE POINTOPOINT { OSPF_PATT->type = OSPF_IT_PTP ; }
 | STRICT NONBROADCAST bool { OSPF_PATT->strictnbma = $3 ; }
 | STUB bool { OSPF_PATT->stub = $2 ; }
 | NEIGHBORS '{' ipa_list '}'
 | AUTHENTICATION NONE { OSPF_PATT->autype = OSPF_AUTH_NONE ; }
 | AUTHENTICATION SIMPLE { OSPF_PATT->autype = OSPF_AUTH_SIMPLE ; }
 | AUTHENTICATION CRYPTOGRAPHIC { OSPF_PATT->autype = OSPF_AUTH_CRYPT ; }
 | RX BUFFER LARGE { OSPF_PATT->rxbuf = OSPF_RXBUF_LARGE ; } 
 | RX BUFFER NORMAL { OSPF_PATT->rxbuf = OSPF_RXBUF_NORMAL ; } 
 | RX BUFFER expr { OSPF_PATT->rxbuf = $3 ; if ($3 < OSPF_RXBUF_MINSIZE) cf_error("Buffer size is too small") ; } 
 | password_list
 ;

pref_list:
 /* empty */
 | pref_list pref_item
 ;

pref_item:
   pref_el
 | pref_hid;

pref_el: prefix ';'
 {
   this_pref = cfg_allocz(sizeof(struct area_net_config));
   add_tail(&this_area->net_list, NODE this_pref);
   this_pref->px.addr = $1.addr;
   this_pref->px.len = $1.len;
 }
;

pref_hid: prefix HIDDEN ';'
 {
   this_pref = cfg_allocz(sizeof(struct area_net_config));
   add_tail(&this_area->net_list, NODE this_pref);
   this_pref->px.addr = $1.addr;
   this_pref->px.len = $1.len;
   this_pref->hidden = 1;
 }
;

ipa_list:
 /* empty */
 | ipa_list ipa_item
 ;

ipa_item:
    ipa_el
  | ipa_ne;
 
ipa_el: IPA ';'
 {
   this_nbma = cfg_allocz(sizeof(struct nbma_node));
   add_tail(&OSPF_PATT->nbma_list, NODE this_nbma);
   this_nbma->ip=$1;
   this_nbma->eligible=0;
 }
;

ipa_ne: IPA ELIGIBLE ';'
 {
   this_nbma = cfg_allocz(sizeof(struct nbma_node));
   add_tail(&OSPF_PATT->nbma_list, NODE this_nbma);
   this_nbma->ip=$1;
   this_nbma->eligible=1;
 }
;

 
ospf_iface_start:
 {
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&this_area->patt_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->cost = COST_D;
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->pollint = POLLINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->priority = PRIORITY_D;
  OSPF_PATT->waitint = WAIT_DMH*HELLOINT_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->dead = 0;
  OSPF_PATT->type = OSPF_IT_UNDEF;
  OSPF_PATT->strictnbma = 0;
  OSPF_PATT->stub = 0;
  init_list(&OSPF_PATT->nbma_list);
  OSPF_PATT->autype = OSPF_AUTH_NONE;
  reset_passwords();
 }
;

ospf_iface_opts:
   /* empty */
 | ospf_iface_opts ospf_iface_item ';'
 ;

ospf_iface_opt_list:
   /* empty */
 | '{' ospf_iface_opts '}'
 ;

ospf_iface:
  ospf_iface_start iface_patt_list ospf_iface_opt_list { finish_iface_config(OSPF_PATT); }
 ;

opttext:
    TEXT
 | /* empty */ { $$ = NULL; }
 ;


cmd_SHOW_OSPF: SHOW OSPF optsym END
{ ospf_sh(proto_get_named($3, &proto_ospf)); };

cmd_SHOW_OSPF_NEIGHBORS: SHOW OSPF NEIGHBORS optsym opttext END
{ ospf_sh_neigh(proto_get_named($4, &proto_ospf), $5); };

cmd_SHOW_OSPF_INTERFACE: SHOW OSPF INTERFACE optsym opttext END
{ ospf_sh_iface(proto_get_named($4, &proto_ospf), $5); };



cmd_SHOW_OSPF_TOPOLOGY: SHOW OSPF TOPOLOGY optsym opttext END
{ ospf_sh_state(proto_get_named($4, &proto_ospf), 0, 1); };

cmd_SHOW_OSPF_TOPOLOGY_ALL: SHOW OSPF TOPOLOGY ALL optsym opttext END
{ ospf_sh_state(proto_get_named($5, &proto_ospf), 0, 0); };



cmd_SHOW_OSPF_STATE: SHOW OSPF STATE optsym opttext END
{ ospf_sh_state(proto_get_named($4, &proto_ospf), 1, 1); };

cmd_SHOW_OSPF_STATE_ALL: SHOW OSPF STATE ALL optsym opttext END
{ ospf_sh_state(proto_get_named($5, &proto_ospf), 1, 0); };

cmd_SHOW_OSPF_LSADB: SHOW OSPF LSADB optsym opttext END
{ ospf_sh_lsadb(proto_get_named($4, &proto_ospf)); };

/* Grammar from ../../proto/pipe/config.Y */


pipe_proto_start: proto_start PIPE {
     this_proto = proto_config_new(&proto_pipe, sizeof(struct pipe_config));
     this_proto->preference = DEF_PREF_PIPE;
     PIPE_CFG->mode = PIPE_TRANSPARENT;
  }
 ;

pipe_proto:
   pipe_proto_start proto_name '{'
 | pipe_proto proto_item ';'
 | pipe_proto PEER TABLE SYM ';' {
     if ($4->class != SYM_TABLE)
       cf_error("Routing table name expected");
     PIPE_CFG->peer = $4->def;
   }
 | pipe_proto MODE OPAQUE ';' { PIPE_CFG->mode = PIPE_OPAQUE; }
 | pipe_proto MODE TRANSPARENT ';' { PIPE_CFG->mode = PIPE_TRANSPARENT; }
 ;

/* Grammar from ../../proto/rip/config.Y */


rip_cfg_start: proto_start RIP {
     this_proto = proto_config_new(&proto_rip, sizeof(struct rip_proto_config));
     rip_init_config(RIP_CFG);
   }
 ;

rip_cfg:
   rip_cfg_start proto_name '{'
 | rip_cfg proto_item ';'
 | rip_cfg INFINITY expr ';'	{ RIP_CFG->infinity = $3; }
 | rip_cfg PORT expr ';'	{ RIP_CFG->port = $3; }
 | rip_cfg PERIOD expr ';'	{ RIP_CFG->period = $3; }
 | rip_cfg GARBAGE TIME expr ';' { RIP_CFG->garbage_time = $4; }
 | rip_cfg TIMEOUT TIME expr ';' { RIP_CFG->timeout_time = $4; }
 | rip_cfg AUTHENTICATION rip_auth ';' {RIP_CFG->authtype = $3; }
 | rip_cfg password_list ';'
 | rip_cfg HONOR ALWAYS ';'    { RIP_CFG->honor = HO_ALWAYS; }
 | rip_cfg HONOR NEIGHBOR ';'    { RIP_CFG->honor = HO_NEIGHBOR; }
 | rip_cfg HONOR NEVER ';'    { RIP_CFG->honor = HO_NEVER; }
 | rip_cfg INTERFACE rip_iface ';'
 ;

rip_auth:
   PLAINTEXT { $$=AT_PLAINTEXT; }
 | MD5 { $$=AT_MD5; }
 | NONE { $$=AT_NONE; }
 ;


rip_mode: 
    BROADCAST { $$=IM_BROADCAST; }
  | MULTICAST { $$=0; }
  | QUIET     { $$=IM_QUIET; }
  | NOLISTEN  { $$=IM_NOLISTEN; }
  | VERSION1  { $$=IM_VERSION1 | IM_BROADCAST; }
 ;

rip_iface_item:
 | METRIC expr { RIP_IPATT->metric = $2; }
 | MODE rip_mode { RIP_IPATT->mode |= $2; }
 ;

rip_iface_opts: 
   /* empty */
 | rip_iface_opts rip_iface_item ';'
 ;

rip_iface_opt_list:
   /* empty */
 | '{' rip_iface_opts '}'
 ;

rip_iface_init:
   /* EMPTY */ {
     this_ipatt = cfg_allocz(sizeof(struct rip_patt));
     add_tail(&RIP_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
     RIP_IPATT->metric = 1;
   }
 ;

rip_iface:
   rip_iface_init iface_patt_list rip_iface_opt_list
 ;


/* Grammar from ../../proto/static/config.Y */


static_proto_start: proto_start STATIC {
     this_proto = proto_config_new(&proto_static, sizeof(struct static_config));
     static_init_config((struct static_config *) this_proto);
  }
 ;

static_proto:
   static_proto_start proto_name '{'
 | static_proto proto_item ';'
 | static_proto stat_route ';'
 ;

stat_route0: ROUTE prefix {
     this_srt = cfg_allocz(sizeof(struct static_route));
     add_tail(&((struct static_config *) this_proto)->other_routes, &this_srt->n);
     this_srt->net = $2.addr;
     this_srt->masklen = $2.len;
  }
 ;

stat_route:
   stat_route0 VIA ipa {
      this_srt->dest = RTD_ROUTER;
      this_srt->via = $3;
   }
 | stat_route0 VIA TEXT {
      this_srt->dest = RTD_DEVICE;
      this_srt->if_name = $3;
      rem_node(&this_srt->n);
      add_tail(&((struct static_config *) this_proto)->iface_routes, &this_srt->n);
   }
 | stat_route0 DROP { this_srt->dest = RTD_BLACKHOLE; }
 | stat_route0 REJECT { this_srt->dest = RTD_UNREACHABLE; }
 | stat_route0 PROHIBIT { this_srt->dest = RTD_PROHIBIT; }
 ;

cmd_SHOW_STATIC: SHOW STATIC optsym END
{ static_show(proto_get_named($3, &proto_static)); } ;


/* Dynamic rules */


conf: ';' | definition | log_config | mrtdump_base | timeformat_base | rtrid | listen | newtab | proto | debug_default | filter_def | filter_eval | function_def ;
cli_cmd: cmd_CONFIGURE | cmd_CONFIGURE_SOFT | cmd_DOWN | cmd_SHOW_STATUS | cmd_SHOW_MEMORY | cmd_SHOW_PROTOCOLS | cmd_SHOW_PROTOCOLS_ALL | cmd_SHOW_INTERFACES | cmd_SHOW_INTERFACES_SUMMARY | cmd_SHOW_ROUTE | cmd_SHOW_SYMBOLS | cmd_DUMP_RESOURCES | cmd_DUMP_SOCKETS | cmd_DUMP_INTERFACES | cmd_DUMP_NEIGHBORS | cmd_DUMP_ATTRIBUTES | cmd_DUMP_ROUTES | cmd_DUMP_PROTOCOLS | cmd_ECHO | cmd_DISABLE | cmd_ENABLE | cmd_RESTART | cmd_RELOAD | cmd_RELOAD_IN | cmd_RELOAD_OUT | cmd_DEBUG | cmd_MRTDUMP | cmd_RESTRICT | cmd_SHOW_OSPF | cmd_SHOW_OSPF_NEIGHBORS | cmd_SHOW_OSPF_INTERFACE | cmd_SHOW_OSPF_TOPOLOGY | cmd_SHOW_OSPF_TOPOLOGY_ALL | cmd_SHOW_OSPF_STATE | cmd_SHOW_OSPF_STATE_ALL | cmd_SHOW_OSPF_LSADB | cmd_SHOW_STATIC ;
proto: kern_proto '}' | kif_proto '}' | dev_proto '}' | bgp_proto '}' { bgp_check(BGP_CFG); }  | ospf_proto '}' | pipe_proto '}' | rip_cfg '}' { RIP_CFG->passwords = get_passwords(); }  | static_proto '}' ;
kern_proto: kern_proto_start proto_name '{' | kern_proto proto_item ';' | kern_proto kern_item ';' | kern_proto nl_item ';' ;
kif_proto: kif_proto_start proto_name '{' | kif_proto proto_item ';' | kif_proto kif_item ';' ;
dynamic_attr: INVALID_TOKEN { $$ = NULL; } | BGP_ORIGIN
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT, T_ENUM_BGP_ORIGIN, EA_CODE(EAP_BGP, BA_ORIGIN)); } | BGP_PATH
	{ $$ = f_new_dynamic_attr(EAF_TYPE_AS_PATH, T_PATH, EA_CODE(EAP_BGP, BA_AS_PATH)); } | BGP_NEXT_HOP
	{ $$ = f_new_dynamic_attr(EAF_TYPE_IP_ADDRESS, T_IP, EA_CODE(EAP_BGP, BA_NEXT_HOP)); } | BGP_MED
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC)); } | BGP_LOCAL_PREF
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_LOCAL_PREF)); } | BGP_ATOMIC_AGGR
	{ $$ = f_new_dynamic_attr(EAF_TYPE_OPAQUE, T_ENUM_EMPTY, EA_CODE(EAP_BGP, BA_ATOMIC_AGGR)); } | BGP_AGGREGATOR
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_AGGREGATOR)); } | BGP_COMMUNITY
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_COMMUNITY)); } | BGP_ORIGINATOR_ID
	{ $$ = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID, T_QUAD, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID)); } | BGP_CLUSTER_LIST
	{ $$ = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_CLUSTER_LIST)); } | OSPF_METRIC1 { $$ = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC1); } | OSPF_METRIC2 { $$ = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC2); } | OSPF_TAG { $$ = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_TAG); } | OSPF_ROUTER_ID { $$ = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID | EAF_TEMP, T_QUAD, EA_OSPF_ROUTER_ID); } | RIP_METRIC { $$ = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_METRIC); } | RIP_TAG { $$ = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_TAG); } ;

%%
/* C Code from ../../conf/confbase.Y */

/* C Code from ../../sysdep/unix/config.Y */

/* C Code from ../../sysdep/unix/krt.Y */

/* C Code from ../../sysdep/linux/netlink/netlink.Y */

/* C Code from ../../nest/config.Y */

/* C Code from ../../proto/bgp/config.Y */

/* C Code from ../../proto/ospf/config.Y */

/* C Code from ../../proto/pipe/config.Y */

/* C Code from ../../proto/rip/config.Y */

/* C Code from ../../proto/static/config.Y */

