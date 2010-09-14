/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 258,
     CLI_MARKER = 259,
     INVALID_TOKEN = 260,
     GEQ = 261,
     LEQ = 262,
     NEQ = 263,
     AND = 264,
     OR = 265,
     PO = 266,
     PC = 267,
     NUM = 268,
     ENUM = 269,
     RTRID = 270,
     IPA = 271,
     SYM = 272,
     TEXT = 273,
     PREFIX_DUMMY = 274,
     DEFINE = 275,
     ON = 276,
     OFF = 277,
     YES = 278,
     NO = 279,
     LOG = 280,
     SYSLOG = 281,
     ALL = 282,
     DEBUG = 283,
     TRACE = 284,
     INFO = 285,
     REMOTE = 286,
     WARNING = 287,
     ERROR = 288,
     AUTH = 289,
     FATAL = 290,
     BUG = 291,
     STDERR = 292,
     SOFT = 293,
     TIMEFORMAT = 294,
     ISO = 295,
     SHORT = 296,
     LONG = 297,
     BASE = 298,
     NAME = 299,
     CONFIGURE = 300,
     DOWN = 301,
     KERNEL = 302,
     PERSIST = 303,
     SCAN = 304,
     TIME = 305,
     LEARN = 306,
     DEVICE = 307,
     ROUTES = 308,
     ASYNC = 309,
     TABLE = 310,
     ROUTER = 311,
     ID = 312,
     PROTOCOL = 313,
     PREFERENCE = 314,
     DISABLED = 315,
     DIRECT = 316,
     INTERFACE = 317,
     IMPORT = 318,
     EXPORT = 319,
     FILTER = 320,
     NONE = 321,
     STATES = 322,
     FILTERS = 323,
     PASSWORD = 324,
     FROM = 325,
     PASSIVE = 326,
     TO = 327,
     EVENTS = 328,
     PACKETS = 329,
     PROTOCOLS = 330,
     INTERFACES = 331,
     PRIMARY = 332,
     STATS = 333,
     COUNT = 334,
     FOR = 335,
     COMMANDS = 336,
     PREEXPORT = 337,
     GENERATE = 338,
     LISTEN = 339,
     BGP = 340,
     V6ONLY = 341,
     ADDRESS = 342,
     PORT = 343,
     PASSWORDS = 344,
     DESCRIPTION = 345,
     RELOAD = 346,
     IN = 347,
     OUT = 348,
     MRTDUMP = 349,
     MESSAGES = 350,
     RESTRICT = 351,
     MEMORY = 352,
     SHOW = 353,
     STATUS = 354,
     SUMMARY = 355,
     ROUTE = 356,
     SYMBOLS = 357,
     DUMP = 358,
     RESOURCES = 359,
     SOCKETS = 360,
     NEIGHBORS = 361,
     ATTRIBUTES = 362,
     ECHO = 363,
     DISABLE = 364,
     ENABLE = 365,
     RESTART = 366,
     FUNCTION = 367,
     PRINT = 368,
     PRINTN = 369,
     UNSET = 370,
     RETURN = 371,
     ACCEPT = 372,
     REJECT = 373,
     QUITBIRD = 374,
     INT = 375,
     BOOL = 376,
     IP = 377,
     PREFIX = 378,
     PAIR = 379,
     QUAD = 380,
     SET = 381,
     STRING = 382,
     BGPMASK = 383,
     BGPPATH = 384,
     CLIST = 385,
     IF = 386,
     THEN = 387,
     ELSE = 388,
     CASE = 389,
     TRUE = 390,
     FALSE = 391,
     GW = 392,
     NET = 393,
     MASK = 394,
     PROTO = 395,
     SOURCE = 396,
     SCOPE = 397,
     CAST = 398,
     DEST = 399,
     LEN = 400,
     DEFINED = 401,
     ADD = 402,
     DELETE = 403,
     CONTAINS = 404,
     RESET = 405,
     PREPEND = 406,
     FIRST = 407,
     LAST = 408,
     MATCH = 409,
     EMPTY = 410,
     WHERE = 411,
     EVAL = 412,
     LOCAL = 413,
     NEIGHBOR = 414,
     AS = 415,
     HOLD = 416,
     CONNECT = 417,
     RETRY = 418,
     KEEPALIVE = 419,
     MULTIHOP = 420,
     STARTUP = 421,
     VIA = 422,
     NEXT = 423,
     HOP = 424,
     SELF = 425,
     DEFAULT = 426,
     PATH = 427,
     METRIC = 428,
     START = 429,
     DELAY = 430,
     FORGET = 431,
     WAIT = 432,
     AFTER = 433,
     BGP_PATH = 434,
     BGP_LOCAL_PREF = 435,
     BGP_MED = 436,
     BGP_ORIGIN = 437,
     BGP_NEXT_HOP = 438,
     BGP_ATOMIC_AGGR = 439,
     BGP_AGGREGATOR = 440,
     BGP_COMMUNITY = 441,
     RR = 442,
     RS = 443,
     CLIENT = 444,
     CLUSTER = 445,
     AS4 = 446,
     ADVERTISE = 447,
     IPV4 = 448,
     CAPABILITIES = 449,
     LIMIT = 450,
     PREFER = 451,
     OLDER = 452,
     MISSING = 453,
     LLADDR = 454,
     DROP = 455,
     IGNORE = 456,
     REFRESH = 457,
     INTERPRET = 458,
     COMMUNITIES = 459,
     BGP_ORIGINATOR_ID = 460,
     BGP_CLUSTER_LIST = 461,
     OSPF = 462,
     AREA = 463,
     OSPF_METRIC1 = 464,
     OSPF_METRIC2 = 465,
     OSPF_TAG = 466,
     OSPF_ROUTER_ID = 467,
     BROADCAST = 468,
     RFC1583COMPAT = 469,
     STUB = 470,
     TICK = 471,
     COST = 472,
     RETRANSMIT = 473,
     HELLO = 474,
     TRANSMIT = 475,
     PRIORITY = 476,
     DEAD = 477,
     NONBROADCAST = 478,
     POINTOPOINT = 479,
     TYPE = 480,
     SIMPLE = 481,
     AUTHENTICATION = 482,
     STRICT = 483,
     CRYPTOGRAPHIC = 484,
     ELIGIBLE = 485,
     POLL = 486,
     NETWORKS = 487,
     HIDDEN = 488,
     VIRTUAL = 489,
     LINK = 490,
     RX = 491,
     BUFFER = 492,
     LARGE = 493,
     NORMAL = 494,
     STUBNET = 495,
     LSADB = 496,
     TOPOLOGY = 497,
     STATE = 498,
     PIPE = 499,
     PEER = 500,
     MODE = 501,
     OPAQUE = 502,
     TRANSPARENT = 503,
     RIP = 504,
     INFINITY = 505,
     PERIOD = 506,
     GARBAGE = 507,
     TIMEOUT = 508,
     MULTICAST = 509,
     QUIET = 510,
     NOLISTEN = 511,
     VERSION1 = 512,
     PLAINTEXT = 513,
     MD5 = 514,
     HONOR = 515,
     NEVER = 516,
     ALWAYS = 517,
     RIP_METRIC = 518,
     RIP_TAG = 519,
     STATIC = 520,
     PROHIBIT = 521
   };
#endif
/* Tokens.  */
#define END 258
#define CLI_MARKER 259
#define INVALID_TOKEN 260
#define GEQ 261
#define LEQ 262
#define NEQ 263
#define AND 264
#define OR 265
#define PO 266
#define PC 267
#define NUM 268
#define ENUM 269
#define RTRID 270
#define IPA 271
#define SYM 272
#define TEXT 273
#define PREFIX_DUMMY 274
#define DEFINE 275
#define ON 276
#define OFF 277
#define YES 278
#define NO 279
#define LOG 280
#define SYSLOG 281
#define ALL 282
#define DEBUG 283
#define TRACE 284
#define INFO 285
#define REMOTE 286
#define WARNING 287
#define ERROR 288
#define AUTH 289
#define FATAL 290
#define BUG 291
#define STDERR 292
#define SOFT 293
#define TIMEFORMAT 294
#define ISO 295
#define SHORT 296
#define LONG 297
#define BASE 298
#define NAME 299
#define CONFIGURE 300
#define DOWN 301
#define KERNEL 302
#define PERSIST 303
#define SCAN 304
#define TIME 305
#define LEARN 306
#define DEVICE 307
#define ROUTES 308
#define ASYNC 309
#define TABLE 310
#define ROUTER 311
#define ID 312
#define PROTOCOL 313
#define PREFERENCE 314
#define DISABLED 315
#define DIRECT 316
#define INTERFACE 317
#define IMPORT 318
#define EXPORT 319
#define FILTER 320
#define NONE 321
#define STATES 322
#define FILTERS 323
#define PASSWORD 324
#define FROM 325
#define PASSIVE 326
#define TO 327
#define EVENTS 328
#define PACKETS 329
#define PROTOCOLS 330
#define INTERFACES 331
#define PRIMARY 332
#define STATS 333
#define COUNT 334
#define FOR 335
#define COMMANDS 336
#define PREEXPORT 337
#define GENERATE 338
#define LISTEN 339
#define BGP 340
#define V6ONLY 341
#define ADDRESS 342
#define PORT 343
#define PASSWORDS 344
#define DESCRIPTION 345
#define RELOAD 346
#define IN 347
#define OUT 348
#define MRTDUMP 349
#define MESSAGES 350
#define RESTRICT 351
#define MEMORY 352
#define SHOW 353
#define STATUS 354
#define SUMMARY 355
#define ROUTE 356
#define SYMBOLS 357
#define DUMP 358
#define RESOURCES 359
#define SOCKETS 360
#define NEIGHBORS 361
#define ATTRIBUTES 362
#define ECHO 363
#define DISABLE 364
#define ENABLE 365
#define RESTART 366
#define FUNCTION 367
#define PRINT 368
#define PRINTN 369
#define UNSET 370
#define RETURN 371
#define ACCEPT 372
#define REJECT 373
#define QUITBIRD 374
#define INT 375
#define BOOL 376
#define IP 377
#define PREFIX 378
#define PAIR 379
#define QUAD 380
#define SET 381
#define STRING 382
#define BGPMASK 383
#define BGPPATH 384
#define CLIST 385
#define IF 386
#define THEN 387
#define ELSE 388
#define CASE 389
#define TRUE 390
#define FALSE 391
#define GW 392
#define NET 393
#define MASK 394
#define PROTO 395
#define SOURCE 396
#define SCOPE 397
#define CAST 398
#define DEST 399
#define LEN 400
#define DEFINED 401
#define ADD 402
#define DELETE 403
#define CONTAINS 404
#define RESET 405
#define PREPEND 406
#define FIRST 407
#define LAST 408
#define MATCH 409
#define EMPTY 410
#define WHERE 411
#define EVAL 412
#define LOCAL 413
#define NEIGHBOR 414
#define AS 415
#define HOLD 416
#define CONNECT 417
#define RETRY 418
#define KEEPALIVE 419
#define MULTIHOP 420
#define STARTUP 421
#define VIA 422
#define NEXT 423
#define HOP 424
#define SELF 425
#define DEFAULT 426
#define PATH 427
#define METRIC 428
#define START 429
#define DELAY 430
#define FORGET 431
#define WAIT 432
#define AFTER 433
#define BGP_PATH 434
#define BGP_LOCAL_PREF 435
#define BGP_MED 436
#define BGP_ORIGIN 437
#define BGP_NEXT_HOP 438
#define BGP_ATOMIC_AGGR 439
#define BGP_AGGREGATOR 440
#define BGP_COMMUNITY 441
#define RR 442
#define RS 443
#define CLIENT 444
#define CLUSTER 445
#define AS4 446
#define ADVERTISE 447
#define IPV4 448
#define CAPABILITIES 449
#define LIMIT 450
#define PREFER 451
#define OLDER 452
#define MISSING 453
#define LLADDR 454
#define DROP 455
#define IGNORE 456
#define REFRESH 457
#define INTERPRET 458
#define COMMUNITIES 459
#define BGP_ORIGINATOR_ID 460
#define BGP_CLUSTER_LIST 461
#define OSPF 462
#define AREA 463
#define OSPF_METRIC1 464
#define OSPF_METRIC2 465
#define OSPF_TAG 466
#define OSPF_ROUTER_ID 467
#define BROADCAST 468
#define RFC1583COMPAT 469
#define STUB 470
#define TICK 471
#define COST 472
#define RETRANSMIT 473
#define HELLO 474
#define TRANSMIT 475
#define PRIORITY 476
#define DEAD 477
#define NONBROADCAST 478
#define POINTOPOINT 479
#define TYPE 480
#define SIMPLE 481
#define AUTHENTICATION 482
#define STRICT 483
#define CRYPTOGRAPHIC 484
#define ELIGIBLE 485
#define POLL 486
#define NETWORKS 487
#define HIDDEN 488
#define VIRTUAL 489
#define LINK 490
#define RX 491
#define BUFFER 492
#define LARGE 493
#define NORMAL 494
#define STUBNET 495
#define LSADB 496
#define TOPOLOGY 497
#define STATE 498
#define PIPE 499
#define PEER 500
#define MODE 501
#define OPAQUE 502
#define TRANSPARENT 503
#define RIP 504
#define INFINITY 505
#define PERIOD 506
#define GARBAGE 507
#define TIMEOUT 508
#define MULTICAST 509
#define QUIET 510
#define NOLISTEN 511
#define VERSION1 512
#define PLAINTEXT 513
#define MD5 514
#define HONOR 515
#define NEVER 516
#define ALWAYS 517
#define RIP_METRIC 518
#define RIP_TAG 519
#define STATIC 520
#define PROHIBIT 521




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 157 "cf-parse.y"
{
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
/* Line 1489 of yacc.c.  */
#line 603 "cf-parse.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE cf_lval;

