/* vi: set sw=4 ts=4: */
/* stty -- change and print terminal line settings
   Copyright (C) 1990-1999 Free Software Foundation, Inc.

   Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
*/
/* Usage: stty [-ag] [-F device] [setting...]

   Options:
   -a Write all current settings to stdout in human-readable form.
   -g Write all current settings to stdout in stty-readable form.
   -F Open and use the specified device instead of stdin

   If no args are given, write to stdout the baud rate and settings that
   have been changed from their defaults.  Mode reading and changes
   are done on the specified device, or stdin if none was specified.

   David MacKenzie <djm@gnu.ai.mit.edu>

   Special for busybox ported by Vladimir Oleynik <dzo@simtreas.ru> 2001

   */

#include "busybox.h"

#ifndef _POSIX_VDISABLE
# define _POSIX_VDISABLE ((unsigned char) 0)
#endif

#define Control(c) ((c) & 0x1f)
/* Canonical values for control characters */
#ifndef CINTR
# define CINTR Control('c')
#endif
#ifndef CQUIT
# define CQUIT 28
#endif
#ifndef CERASE
# define CERASE 127
#endif
#ifndef CKILL
# define CKILL Control('u')
#endif
#ifndef CEOF
# define CEOF Control('d')
#endif
#ifndef CEOL
# define CEOL _POSIX_VDISABLE
#endif
#ifndef CSTART
# define CSTART Control('q')
#endif
#ifndef CSTOP
# define CSTOP Control('s')
#endif
#ifndef CSUSP
# define CSUSP Control('z')
#endif
#if defined(VEOL2) && !defined(CEOL2)
# define CEOL2 _POSIX_VDISABLE
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name */
#if defined(VSUSP) && !defined(VSWTCH)
# define VSWTCH VSUSP
# define CSWTCH CSUSP
#endif
#if defined(VSWTCH) && !defined(CSWTCH)
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#if defined (__sparc__) && defined (__svr4__)
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#if defined(VWERSE) && !defined (VWERASE)       /* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#if defined(VDSUSP) && !defined (CDSUSP)
# define CDSUSP Control('y')
#endif
#if !defined(VREPRINT) && defined(VRPRNT)       /* Irix 4.0.5 */
# define VREPRINT VRPRNT
#endif
#if defined(VREPRINT) && !defined(CRPRNT)
# define CRPRNT Control('r')
#endif
#if defined(VWERASE) && !defined(CWERASE)
# define CWERASE Control('w')
#endif
#if defined(VLNEXT) && !defined(CLNEXT)
# define CLNEXT Control('v')
#endif
#if defined(VDISCARD) && !defined(VFLUSHO)
# define VFLUSHO VDISCARD
#endif
#if defined(VFLUSH) && !defined(VFLUSHO)        /* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#if defined(CTLECH) && !defined(ECHOCTL)        /* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#if defined(TCTLECH) && !defined(ECHOCTL)       /* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#if defined(CRTKIL) && !defined(ECHOKE)         /* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#if defined(VFLUSHO) && !defined(CFLUSHO)
# define CFLUSHO Control('o')
#endif
#if defined(VSTATUS) && !defined(CSTATUS)
# define CSTATUS Control('t')
#endif

/* Which speeds to set */
enum speed_setting {
	input_speed, output_speed, both_speeds
};

/* Which member(s) of 'struct termios' a mode uses */
enum {
	/* Do NOT change the order or values, as mode_type_flag()
	 * depends on them */
	control, input, output, local, combination
};

static const char evenp     [] = "evenp";
static const char raw       [] = "raw";
static const char stty_min  [] = "min";
static const char stty_time [] = "time";
static const char stty_swtch[] = "swtch";
static const char stty_eol  [] = "eol";
static const char stty_eof  [] = "eof";
static const char parity    [] = "parity";
static const char stty_oddp [] = "oddp";
static const char stty_nl   [] = "nl";
static const char stty_ek   [] = "ek";
static const char stty_sane [] = "sane";
static const char cbreak    [] = "cbreak";
static const char stty_pass8[] = "pass8";
static const char litout    [] = "litout";
static const char cooked    [] = "cooked";
static const char decctlq   [] = "decctlq";
static const char stty_tabs [] = "tabs";
static const char stty_lcase[] = "lcase";
static const char stty_LCASE[] = "LCASE";
static const char stty_crt  [] = "crt";
static const char stty_dec  [] = "dec";

/* Flags for 'struct mode_info' */
#define SANE_SET 1              /* Set in 'sane' mode                  */
#define SANE_UNSET 2            /* Unset in 'sane' mode                */
#define REV 4                   /* Can be turned off by prepending '-' */
#define OMIT 8                  /* Don't display value                 */

/* Each mode */
struct mode_info {
	const char * const name;      /* Name given on command line           */
	const unsigned char type;     /* Which structure element to change    */
	const unsigned char flags;    /* Setting and display options          */
	/* were using short here, but ppc32 was unhappy: */
	const tcflag_t mask;          /* Other bits to turn off for this mode */
	const tcflag_t bits;          /* Bits to set for this mode            */
};

/* We can optimize it further by using name[8] instead of char *name */
/* but beware of "if (info->name == evenp)" checks! */
/* Need to replace them with "if (info == &mode_info[EVENP_INDX])" */

#define MI_ENTRY(N,T,F,B,M) { N, T, F, M, B }

static const struct mode_info mode_info[] = {
	MI_ENTRY("parenb",   control,     REV,               PARENB,     0 ),
	MI_ENTRY("parodd",   control,     REV,               PARODD,     0 ),
	MI_ENTRY("cs5",      control,     0,                 CS5,     CSIZE),
	MI_ENTRY("cs6",      control,     0,                 CS6,     CSIZE),
	MI_ENTRY("cs7",      control,     0,                 CS7,     CSIZE),
	MI_ENTRY("cs8",      control,     0,                 CS8,     CSIZE),
	MI_ENTRY("hupcl",    control,     REV,               HUPCL,      0 ),
	MI_ENTRY("hup",      control,     REV        | OMIT, HUPCL,      0 ),
	MI_ENTRY("cstopb",   control,     REV,               CSTOPB,     0 ),
	MI_ENTRY("cread",    control,     SANE_SET   | REV,  CREAD,      0 ),
	MI_ENTRY("clocal",   control,     REV,               CLOCAL,     0 ),
#ifdef CRTSCTS
	MI_ENTRY("crtscts",  control,     REV,               CRTSCTS,    0 ),
#endif
	MI_ENTRY("ignbrk",   input,       SANE_UNSET | REV,  IGNBRK,     0 ),
	MI_ENTRY("brkint",   input,       SANE_SET   | REV,  BRKINT,     0 ),
	MI_ENTRY("ignpar",   input,       REV,               IGNPAR,     0 ),
	MI_ENTRY("parmrk",   input,       REV,               PARMRK,     0 ),
	MI_ENTRY("inpck",    input,       REV,               INPCK,      0 ),
	MI_ENTRY("istrip",   input,       REV,               ISTRIP,     0 ),
	MI_ENTRY("inlcr",    input,       SANE_UNSET | REV,  INLCR,      0 ),
	MI_ENTRY("igncr",    input,       SANE_UNSET | REV,  IGNCR,      0 ),
	MI_ENTRY("icrnl",    input,       SANE_SET   | REV,  ICRNL,      0 ),
	MI_ENTRY("ixon",     input,       REV,               IXON,       0 ),
	MI_ENTRY("ixoff",    input,       SANE_UNSET | REV,  IXOFF,      0 ),
	MI_ENTRY("tandem",   input,       REV        | OMIT, IXOFF,      0 ),
#ifdef IUCLC
	MI_ENTRY("iuclc",    input,       SANE_UNSET | REV,  IUCLC,      0 ),
#endif
#ifdef IXANY
	MI_ENTRY("ixany",    input,       SANE_UNSET | REV,  IXANY,      0 ),
#endif
#ifdef IMAXBEL
	MI_ENTRY("imaxbel",  input,       SANE_SET   | REV,  IMAXBEL,    0 ),
#endif
	MI_ENTRY("opost",    output,      SANE_SET   | REV,  OPOST,      0 ),
#ifdef OLCUC
	MI_ENTRY("olcuc",    output,      SANE_UNSET | REV,  OLCUC,      0 ),
#endif
#ifdef OCRNL
	MI_ENTRY("ocrnl",    output,      SANE_UNSET | REV,  OCRNL,      0 ),
#endif
#ifdef ONLCR
	MI_ENTRY("onlcr",    output,      SANE_SET   | REV,  ONLCR,      0 ),
#endif
#ifdef ONOCR
	MI_ENTRY("onocr",    output,      SANE_UNSET | REV,  ONOCR,      0 ),
#endif
#ifdef ONLRET
	MI_ENTRY("onlret",   output,      SANE_UNSET | REV,  ONLRET,     0 ),
#endif
#ifdef OFILL
	MI_ENTRY("ofill",    output,      SANE_UNSET | REV,  OFILL,      0 ),
#endif
#ifdef OFDEL
	MI_ENTRY("ofdel",    output,      SANE_UNSET | REV,  OFDEL,      0 ),
#endif
#ifdef NLDLY
	MI_ENTRY("nl1",      output,      SANE_UNSET,        NL1,     NLDLY),
	MI_ENTRY("nl0",      output,      SANE_SET,          NL0,     NLDLY),
#endif
#ifdef CRDLY
	MI_ENTRY("cr3",      output,      SANE_UNSET,        CR3,     CRDLY),
	MI_ENTRY("cr2",      output,      SANE_UNSET,        CR2,     CRDLY),
	MI_ENTRY("cr1",      output,      SANE_UNSET,        CR1,     CRDLY),
	MI_ENTRY("cr0",      output,      SANE_SET,          CR0,     CRDLY),
#endif

#ifdef TABDLY
	MI_ENTRY("tab3",     output,      SANE_UNSET,        TAB3,   TABDLY),
	MI_ENTRY("tab2",     output,      SANE_UNSET,        TAB2,   TABDLY),
	MI_ENTRY("tab1",     output,      SANE_UNSET,        TAB1,   TABDLY),
	MI_ENTRY("tab0",     output,      SANE_SET,          TAB0,   TABDLY),
#else
# ifdef OXTABS
	MI_ENTRY("tab3",     output,      SANE_UNSET,        OXTABS,     0 ),
# endif
#endif

#ifdef BSDLY
	MI_ENTRY("bs1",      output,      SANE_UNSET,        BS1,     BSDLY),
	MI_ENTRY("bs0",      output,      SANE_SET,          BS0,     BSDLY),
#endif
#ifdef VTDLY
	MI_ENTRY("vt1",      output,      SANE_UNSET,        VT1,     VTDLY),
	MI_ENTRY("vt0",      output,      SANE_SET,          VT0,     VTDLY),
#endif
#ifdef FFDLY
	MI_ENTRY("ff1",      output,      SANE_UNSET,        FF1,     FFDLY),
	MI_ENTRY("ff0",      output,      SANE_SET,          FF0,     FFDLY),
#endif
	MI_ENTRY("isig",     local,       SANE_SET   | REV,  ISIG,       0 ),
	MI_ENTRY("icanon",   local,       SANE_SET   | REV,  ICANON,     0 ),
#ifdef IEXTEN
	MI_ENTRY("iexten",   local,       SANE_SET   | REV,  IEXTEN,     0 ),
#endif
	MI_ENTRY("echo",     local,       SANE_SET   | REV,  ECHO,       0 ),
	MI_ENTRY("echoe",    local,       SANE_SET   | REV,  ECHOE,      0 ),
	MI_ENTRY("crterase", local,       REV        | OMIT, ECHOE,      0 ),
	MI_ENTRY("echok",    local,       SANE_SET   | REV,  ECHOK,      0 ),
	MI_ENTRY("echonl",   local,       SANE_UNSET | REV,  ECHONL,     0 ),
	MI_ENTRY("noflsh",   local,       SANE_UNSET | REV,  NOFLSH,     0 ),
#ifdef XCASE
	MI_ENTRY("xcase",    local,       SANE_UNSET | REV,  XCASE,      0 ),
#endif
#ifdef TOSTOP
	MI_ENTRY("tostop",   local,       SANE_UNSET | REV,  TOSTOP,     0 ),
#endif
#ifdef ECHOPRT
	MI_ENTRY("echoprt",  local,       SANE_UNSET | REV,  ECHOPRT,    0 ),
	MI_ENTRY("prterase", local,       REV | OMIT,        ECHOPRT,    0 ),
#endif
#ifdef ECHOCTL
	MI_ENTRY("echoctl",  local,       SANE_SET   | REV,  ECHOCTL,    0 ),
	MI_ENTRY("ctlecho",  local,       REV        | OMIT, ECHOCTL,    0 ),
#endif
#ifdef ECHOKE
	MI_ENTRY("echoke",   local,       SANE_SET   | REV,  ECHOKE,     0 ),
	MI_ENTRY("crtkill",  local,       REV        | OMIT, ECHOKE,     0 ),
#endif
	MI_ENTRY(evenp,      combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(parity,     combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(stty_oddp,  combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(stty_nl,    combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(stty_ek,    combination, OMIT,              0,          0 ),
	MI_ENTRY(stty_sane,  combination, OMIT,              0,          0 ),
	MI_ENTRY(cooked,     combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(raw,        combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(stty_pass8, combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(litout,     combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(cbreak,     combination, REV        | OMIT, 0,          0 ),
#ifdef IXANY
	MI_ENTRY(decctlq,    combination, REV        | OMIT, 0,          0 ),
#endif
#if defined(TABDLY) || defined(OXTABS)
	MI_ENTRY(stty_tabs,  combination, REV        | OMIT, 0,          0 ),
#endif
#if defined(XCASE) && defined(IUCLC) && defined(OLCUC)
	MI_ENTRY(stty_lcase, combination, REV        | OMIT, 0,          0 ),
	MI_ENTRY(stty_LCASE, combination, REV        | OMIT, 0,          0 ),
#endif
	MI_ENTRY(stty_crt,   combination, OMIT,              0,          0 ),
	MI_ENTRY(stty_dec,   combination, OMIT,              0,          0 ),
};

enum {
	NUM_mode_info = (sizeof(mode_info) / sizeof(mode_info[0]))
};

/* Control character settings */
struct control_info {
	const char * const name;               /* Name given on command line */
	const unsigned char saneval;          /* Value to set for 'stty sane' */
	const unsigned char offset;           /* Offset in c_cc */
};

/* Control characters */

static const struct control_info control_info[] = {
	{"intr",     CINTR,   VINTR},
	{"quit",     CQUIT,   VQUIT},
	{"erase",    CERASE,  VERASE},
	{"kill",     CKILL,   VKILL},
	{stty_eof,   CEOF,    VEOF},
	{stty_eol,   CEOL,    VEOL},
#ifdef VEOL2
	{"eol2",     CEOL2,   VEOL2},
#endif
#ifdef VSWTCH
	{stty_swtch, CSWTCH,  VSWTCH},
#endif
	{"start",    CSTART,  VSTART},
	{"stop",     CSTOP,   VSTOP},
	{"susp",     CSUSP,   VSUSP},
#ifdef VDSUSP
	{"dsusp",    CDSUSP,  VDSUSP},
#endif
#ifdef VREPRINT
	{"rprnt",    CRPRNT,  VREPRINT},
#endif
#ifdef VWERASE
	{"werase",   CWERASE, VWERASE},
#endif
#ifdef VLNEXT
	{"lnext",    CLNEXT,  VLNEXT},
#endif
#ifdef VFLUSHO
	{"flush",    CFLUSHO, VFLUSHO},
#endif
#ifdef VSTATUS
	{"status",   CSTATUS, VSTATUS},
#endif
	/* These must be last because of the display routines */
	{stty_min,   1,       VMIN},
	{stty_time,  0,       VTIME},
};

enum {
	NUM_control_info = (sizeof(control_info) / sizeof(control_info[0]))
};

/* The width of the screen, for output wrapping */
static unsigned max_col = 80; /* default */
/* Current position, to know when to wrap */
static unsigned current_col;
static const char *device_name = bb_msg_standard_input;

/* Return a string that is the printable representation of character CH */
/* Adapted from 'cat' by Torbjorn Granlund */
static const char *visible(unsigned int ch)
{
	static char buf[10];
	char *bpout = buf;

	if (ch == _POSIX_VDISABLE)
		return "<undef>";

	if (ch >= 128) {
		ch -= 128;
		*bpout++ = 'M';
		*bpout++ = '-';
	}

	if (ch < 32) {
		*bpout++ = '^';
		*bpout++ = ch + 64;
	} else if (ch < 127) {
		*bpout++ = ch;
	} else {
		*bpout++ = '^';
		*bpout++ = '?';
	}

	*bpout = '\0';
	return buf;
}

static tcflag_t *mode_type_flag(unsigned type, const struct termios *mode)
{
	static const unsigned char tcflag_offsets[] = {
		offsetof(struct termios, c_cflag), /* control */
		offsetof(struct termios, c_iflag), /* input */
		offsetof(struct termios, c_oflag), /* output */
		offsetof(struct termios, c_lflag), /* local */
	};

	if (type <= local) {
		return (tcflag_t*) (((char*)mode) + tcflag_offsets[type]);
	}
	return NULL;
}

static void set_speed_or_die(enum speed_setting type, const char * const arg,
					struct termios * const mode)
{
	speed_t baud;

	baud = tty_value_to_baud(xatou(arg));

	if (type != output_speed) {     /* either input or both */
		cfsetispeed(mode, baud);
	}
	if (type != input_speed) {      /* either output or both */
		cfsetospeed(mode, baud);
	}
}

static ATTRIBUTE_NORETURN void perror_on_device_and_die(const char *fmt)
{
	bb_perror_msg_and_die(fmt, device_name);
}

static void perror_on_device(const char *fmt)
{
	bb_perror_msg(fmt, device_name);
}

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line */
static void wrapf(const char *message, ...)
{
	char buf[128];
	va_list args;
	int buflen;

	va_start(args, message);
	buflen = vsnprintf(buf, sizeof(buf), message, args);
	va_end(args);
	/* We seem to be called only with suitable lengths, but check if
	   somebody failed to adhere to this assumption just to be sure.  */
	if (!buflen || buflen >= sizeof(buf)) return;

	if (current_col > 0) {
		current_col++;
		if (buf[0] != '\n') {
			if (current_col + buflen >= max_col) {
				putchar('\n');
				current_col = 0;
			} else
				putchar(' ');
		}
	}
	fputs(buf, stdout);
	current_col += buflen;
	if (buf[buflen-1] == '\n')
		current_col = 0;
}

static void set_window_size(const int rows, const int cols)
{
	struct winsize win = { 0, 0, 0, 0};

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win)) {
		if (errno != EINVAL) {
			goto bail;
		}
		memset(&win, 0, sizeof(win));
	}

	if (rows >= 0)
		win.ws_row = rows;
	if (cols >= 0)
		win.ws_col = cols;

	if (ioctl(STDIN_FILENO, TIOCSWINSZ, (char *) &win))
bail:
		perror_on_device("%s");
}

static void display_window_size(const int fancy)
{
	const char *fmt_str = "%s\0%s: no size information for this device";
	unsigned width, height;

	if (get_terminal_width_height(STDIN_FILENO, &width, &height)) {
		if ((errno != EINVAL) || ((fmt_str += 2), !fancy)) {
			perror_on_device(fmt_str);
		}
	} else {
		wrapf(fancy ? "rows %d; columns %d;" : "%d %d\n",
				height, width);
	}
}

static const struct suffix_mult stty_suffixes[] = {
	{"b",  512 },
	{"k",  1024},
	{"B",  1024},
	{NULL, 0   }
};

static const struct mode_info *find_mode(const char *name)
{
	int i;
	for (i = 0; i < NUM_mode_info; ++i)
		if (!strcmp(name, mode_info[i].name))
			return &mode_info[i];
	return 0;
}

static const struct control_info *find_control(const char *name)
{
	int i;
	for (i = 0; i < NUM_control_info; ++i)
		if (!strcmp(name, control_info[i].name))
			return &control_info[i];
	return 0;
}

enum {
	param_need_arg = 0x80,
	param_line   = 1 | 0x80,
	param_rows   = 2 | 0x80,
	param_cols   = 3 | 0x80,
	param_size   = 4,
	param_speed  = 5,
	param_ispeed = 6 | 0x80,
	param_ospeed = 7 | 0x80,
};

static int find_param(const char * const name)
{
	const char * const params[] = {
		"line",
		"rows",
		"cols",
		"columns",
		"size",
		"speed",
		"ispeed",
		"ospeed",
		NULL
	};
	int i = index_in_str_array(params, name);
	if (i) {
		if (!(i == 4 || i == 5))
			i |= 0x80;
	}
	return i;
}

static int recover_mode(const char *arg, struct termios *mode)
{
	int i, n;
	unsigned int chr;
	unsigned long iflag, oflag, cflag, lflag;

	/* Scan into temporaries since it is too much trouble to figure out
	   the right format for 'tcflag_t' */
	if (sscanf(arg, "%lx:%lx:%lx:%lx%n",
			   &iflag, &oflag, &cflag, &lflag, &n) != 4)
		return 0;
	mode->c_iflag = iflag;
	mode->c_oflag = oflag;
	mode->c_cflag = cflag;
	mode->c_lflag = lflag;
	arg += n;
	for (i = 0; i < NCCS; ++i) {
		if (sscanf(arg, ":%x%n", &chr, &n) != 1)
			return 0;
		mode->c_cc[i] = chr;
		arg += n;
	}

	/* Fail if there are too many fields */
	if (*arg != '\0')
		return 0;

	return 1;
}

static void display_recoverable(const struct termios *mode,
								const int ATTRIBUTE_UNUSED dummy)
{
	int i;
	printf("%lx:%lx:%lx:%lx",
		   (unsigned long) mode->c_iflag, (unsigned long) mode->c_oflag,
		   (unsigned long) mode->c_cflag, (unsigned long) mode->c_lflag);
	for (i = 0; i < NCCS; ++i)
		printf(":%x", (unsigned int) mode->c_cc[i]);
	putchar('\n');
}

static void display_speed(const struct termios *mode, int fancy)
{
	                     //01234567 8 9
	const char *fmt_str = "%lu %lu\n\0ispeed %lu baud; ospeed %lu baud;";
	unsigned long ispeed, ospeed;

	ospeed = ispeed = cfgetispeed(mode);
	if (ispeed == 0 || ispeed == (ospeed = cfgetospeed(mode))) {
		ispeed = ospeed;                /* in case ispeed was 0 */
	                 //0123 4 5 6 7 8 9
		fmt_str = "%lu\n\0\0\0\0\0speed %lu baud;";
	}
	if (fancy) fmt_str += 9;
	wrapf(fmt_str, tty_baud_to_value(ispeed), tty_baud_to_value(ospeed));
}

static void do_display(const struct termios *mode, const int all)
{
	int i;
	tcflag_t *bitsp;
	unsigned long mask;
	int prev_type = control;

	display_speed(mode, 1);
	if (all)
		display_window_size(1);
#ifdef HAVE_C_LINE
	wrapf("line = %d;\n", mode->c_line);
#else
	wrapf("\n");
#endif

	for (i = 0; control_info[i].name != stty_min; ++i) {
		/* If swtch is the same as susp, don't print both */
#if VSWTCH == VSUSP
		if (control_info[i].name == stty_swtch)
			continue;
#endif
		/* If eof uses the same slot as min, only print whichever applies */
#if VEOF == VMIN
		if ((mode->c_lflag & ICANON) == 0
			&& (control_info[i].name == stty_eof
				|| control_info[i].name == stty_eol)) continue;
#endif
		wrapf("%s = %s;", control_info[i].name,
			  visible(mode->c_cc[control_info[i].offset]));
	}
#if VEOF == VMIN
	if ((mode->c_lflag & ICANON) == 0)
#endif
		wrapf("min = %d; time = %d;", mode->c_cc[VMIN], mode->c_cc[VTIME]);
	if (current_col) wrapf("\n");

	for (i = 0; i < NUM_mode_info; ++i) {
		if (mode_info[i].flags & OMIT)
			continue;
		if (mode_info[i].type != prev_type) {
			/* wrapf("\n"); */
			if (current_col) wrapf("\n");
			prev_type = mode_info[i].type;
		}

		bitsp = mode_type_flag(mode_info[i].type, mode);
		mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
		if ((*bitsp & mask) == mode_info[i].bits) {
			if (all || (mode_info[i].flags & SANE_UNSET))
				wrapf("%s", mode_info[i].name);
		} else {
			if ((all && mode_info[i].flags & REV) ||
				 (!all &&
				  (mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV)))
				wrapf("-%s", mode_info[i].name);
		}
	}
	if (current_col) wrapf("\n");
}

static void sane_mode(struct termios *mode)
{
	int i;
	tcflag_t *bitsp;

	for (i = 0; i < NUM_control_info; ++i) {
#if VMIN == VEOF
		if (control_info[i].name == stty_min)
			break;
#endif
		mode->c_cc[control_info[i].offset] = control_info[i].saneval;
	}

	for (i = 0; i < NUM_mode_info; ++i) {
		if (mode_info[i].flags & SANE_SET) {
			bitsp = mode_type_flag(mode_info[i].type, mode);
			*bitsp = (*bitsp & ~((unsigned long)mode_info[i].mask))
				| mode_info[i].bits;
		} else if (mode_info[i].flags & SANE_UNSET) {
			bitsp = mode_type_flag(mode_info[i].type, mode);
			*bitsp = *bitsp & ~((unsigned long)mode_info[i].mask)
				& ~mode_info[i].bits;
		}
	}
}

/* Save set_mode from #ifdef forest plague */
#ifndef ONLCR
#define ONLCR 0
#endif
#ifndef OCRNL
#define OCRNL 0
#endif
#ifndef ONLRET
#define ONLRET 0
#endif
#ifndef XCASE
#define XCASE 0
#endif
#ifndef IXANY
#define IXANY 0
#endif
#ifndef TABDLY
#define TABDLY 0
#endif
#ifndef OXTABS
#define OXTABS 0
#endif
#ifndef IUCLC
#define IUCLC 0
#endif
#ifndef OLCUC
#define OLCUC 0
#endif
#ifndef ECHOCTL
#define ECHOCTL 0
#endif
#ifndef ECHOKE
#define ECHOKE 0
#endif

static void set_mode(const struct mode_info *info, int reversed,
					struct termios *mode)
{
	tcflag_t *bitsp;

	bitsp = mode_type_flag(info->type, mode);

	if (bitsp) {
		if (reversed)
			*bitsp = *bitsp & ~info->mask & ~info->bits;
		else
			*bitsp = (*bitsp & ~info->mask) | info->bits;
		return;
	}

	/* Combination mode */
	if (info->name == evenp || info->name == parity) {
		if (reversed)
			mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
		else
			mode->c_cflag =	(mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
	} else if (info->name == stty_oddp) {
		if (reversed)
			mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
		else
			mode->c_cflag =	(mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
	} else if (info->name == stty_nl) {
		if (reversed) {
			mode->c_iflag = (mode->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
			mode->c_oflag = (mode->c_oflag | ONLCR)	& ~OCRNL & ~ONLRET;
		} else {
			mode->c_iflag = mode->c_iflag & ~ICRNL;
			if (ONLCR) mode->c_oflag = mode->c_oflag & ~ONLCR;
		}
	} else if (info->name == stty_ek) {
		mode->c_cc[VERASE] = CERASE;
		mode->c_cc[VKILL] = CKILL;
	} else if (info->name == stty_sane) {
		sane_mode(mode);
	}
	else if (info->name == cbreak) {
		if (reversed)
			mode->c_lflag |= ICANON;
		else
			mode->c_lflag &= ~ICANON;
	} else if (info->name == stty_pass8) {
		if (reversed) {
			mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
			mode->c_iflag |= ISTRIP;
		} else {
			mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
			mode->c_iflag &= ~ISTRIP;
		}
	} else if (info->name == litout) {
		if (reversed) {
			mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
			mode->c_iflag |= ISTRIP;
			mode->c_oflag |= OPOST;
		} else {
			mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
			mode->c_iflag &= ~ISTRIP;
			mode->c_oflag &= ~OPOST;
		}
	} else if (info->name == raw || info->name == cooked) {
		if ((info->name[0] == 'r' && reversed)
			|| (info->name[0] == 'c' && !reversed)) {
			/* Cooked mode */
			mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
			mode->c_oflag |= OPOST;
			mode->c_lflag |= ISIG | ICANON;
#if VMIN == VEOF
			mode->c_cc[VEOF] = CEOF;
#endif
#if VTIME == VEOL
			mode->c_cc[VEOL] = CEOL;
#endif
		} else {
			/* Raw mode */
			mode->c_iflag = 0;
			mode->c_oflag &= ~OPOST;
			mode->c_lflag &= ~(ISIG | ICANON | XCASE);
			mode->c_cc[VMIN] = 1;
			mode->c_cc[VTIME] = 0;
		}
	}
	else if (IXANY && info->name == decctlq) {
		if (reversed)
			mode->c_iflag |= IXANY;
		else
			mode->c_iflag &= ~IXANY;
	}
	else if (TABDLY && info->name == stty_tabs) {
		if (reversed)
			mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
		else
			mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
	}
	else if (OXTABS && info->name == stty_tabs) {
		if (reversed)
			mode->c_oflag |= OXTABS;
		else
			mode->c_oflag &= ~OXTABS;
	}
	else if (XCASE && IUCLC && OLCUC
	&& (info->name == stty_lcase || info->name == stty_LCASE)) {
		if (reversed) {
			mode->c_lflag &= ~XCASE;
			mode->c_iflag &= ~IUCLC;
			mode->c_oflag &= ~OLCUC;
		} else {
			mode->c_lflag |= XCASE;
			mode->c_iflag |= IUCLC;
			mode->c_oflag |= OLCUC;
		}
	}
	else if (info->name == stty_crt) {
		mode->c_lflag |= ECHOE | ECHOCTL | ECHOKE;
	}
	else if (info->name == stty_dec) {
		mode->c_cc[VINTR] = 3; /* ^C */
		mode->c_cc[VERASE] = 127; /* DEL */
		mode->c_cc[VKILL] = 21; /* ^U */
		mode->c_lflag |= ECHOE | ECHOCTL | ECHOKE;
		if (IXANY) mode->c_iflag &= ~IXANY;
	}
}

static void set_control_char_or_die(const struct control_info *info,
			const char *arg, struct termios *mode)
{
	unsigned char value;

	if (info->name == stty_min || info->name == stty_time)
		value = xatoul_range_sfx(arg, 0, 0xff, stty_suffixes);
	else if (arg[0] == '\0' || arg[1] == '\0')
		value = arg[0];
	else if (!strcmp(arg, "^-") || !strcmp(arg, "undef"))
		value = _POSIX_VDISABLE;
	else if (arg[0] == '^') { /* Ignore any trailing junk (^Cjunk) */
		value = arg[1] & 0x1f; /* Non-letters get weird results */
		if (arg[1] == '?')
			value = 127;
	} else
		value = xatoul_range_sfx(arg, 0, 0xff, stty_suffixes);
	mode->c_cc[info->offset] = value;
}

#define STTY_require_set_attr   (1<<0)
#define STTY_speed_was_set      (1<<1)
#define STTY_verbose_output     (1<<2)
#define STTY_recoverable_output (1<<3)
#define STTY_noargs             (1<<4)
int stty_main(int argc, char **argv)
{
	struct termios mode;
	void (*output_func)(const struct termios *, const int);
	const char *file_name = NULL;
	int display_all = 0;
	int stty_state;
	int k;

	stty_state = STTY_noargs;
	output_func = do_display;

	/* First pass: only parse/verify command line params */
	k = 0;
	while (argv[++k]) {
		const struct mode_info *mp;
		const struct control_info *cp;
		const char *arg = argv[k];
		const char *argnext = argv[k+1];
		int param;

		if (arg[0] == '-') {
			int i;
			mp = find_mode(arg+1);
			if (mp) {
				if (!(mp->flags & REV))
					goto invalid_argument;
				stty_state &= ~STTY_noargs;
				continue;
			}
			/* It is an option - parse it */
			i = 0;
			while (arg[++i]) {
				switch (arg[i]) {
				case 'a':
					stty_state |= STTY_verbose_output;
					output_func = do_display;
					display_all = 1;
					break;
				case 'g':
					stty_state |= STTY_recoverable_output;
					output_func = display_recoverable;
					break;
				case 'F':
					if (file_name)
						bb_error_msg_and_die("only one device may be specified");
					file_name = &arg[i+1]; /* "-Fdevice" ? */
					if (!file_name[0]) { /* nope, "-F device" */
						int p = k+1; /* argv[p] is argnext */
						file_name = argnext;
						if (!file_name)
							bb_error_msg_and_die(bb_msg_requires_arg, "-F");
						/* remove -F param from arg[vc] */
						--argc;
						while (argv[p]) { argv[p] = argv[p+1]; ++p; }
					}
					goto end_option;
				default:
					goto invalid_argument;
				}
			}
end_option:
			continue;
		}

		mp = find_mode(arg);
		if (mp) {
			stty_state &= ~STTY_noargs;
			continue;
		}

		cp = find_control(arg);
		if (cp) {
			if (!argnext)
				bb_error_msg_and_die(bb_msg_requires_arg, arg);
			/* called for the side effect of xfunc death only */
			set_control_char_or_die(cp, argnext, &mode);
			stty_state &= ~STTY_noargs;
			++k;
			continue;
		}

		param = find_param(arg);
		if (param & param_need_arg) {
			if (!argnext)
				bb_error_msg_and_die(bb_msg_requires_arg, arg);
			++k;
		}

		switch (param) {
#ifdef HAVE_C_LINE
		case param_line:
# ifndef TIOCGWINSZ
			xatoul_range_sfx(argnext, 1, INT_MAX, stty_suffixes);
			break;
# endif /* else fall-through */
#endif
#ifdef TIOCGWINSZ
		case param_rows:
		case param_cols:
			xatoul_range_sfx(argnext, 1, INT_MAX, stty_suffixes);
			break;
		case param_size:
#endif
		case param_speed:
			break;
		case param_ispeed:
			/* called for the side effect of xfunc death only */
			set_speed_or_die(input_speed, argnext, &mode);
			break;
		case param_ospeed:
			/* called for the side effect of xfunc death only */
			set_speed_or_die(output_speed, argnext, &mode);
			break;
		default:
			if (recover_mode(arg, &mode) == 1) break;
			if (tty_value_to_baud(xatou(arg)) != (speed_t) -1) break;
invalid_argument:
			bb_error_msg_and_die("invalid argument '%s'", arg);
		}
		stty_state &= ~STTY_noargs;
	}

	/* Specifying both -a and -g is an error */
	if ((stty_state & (STTY_verbose_output | STTY_recoverable_output)) ==
		(STTY_verbose_output | STTY_recoverable_output))
		bb_error_msg_and_die("verbose and stty-readable output styles are mutually exclusive");
	/* Specifying -a or -g with non-options is an error */
	if (!(stty_state & STTY_noargs) &&
		(stty_state & (STTY_verbose_output | STTY_recoverable_output)))
		bb_error_msg_and_die("modes may not be set when specifying an output style");

	/* Now it is safe to start doing things */
	if (file_name) {
		int fd, fdflags;
		device_name = file_name;
		fd = xopen(device_name, O_RDONLY | O_NONBLOCK);
		if (fd != STDIN_FILENO) {
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		fdflags = fcntl(STDIN_FILENO, F_GETFL);
		if (fdflags < 0 ||
			fcntl(STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
			perror_on_device_and_die("%s: cannot reset non-blocking mode");
	}

	/* Initialize to all zeroes so there is no risk memcmp will report a
	   spurious difference in an uninitialized portion of the structure */
	memset(&mode, 0, sizeof(mode));
	if (tcgetattr(STDIN_FILENO, &mode))
		perror_on_device_and_die("%s");

	if (stty_state & (STTY_verbose_output | STTY_recoverable_output | STTY_noargs)) {
		get_terminal_width_height(STDOUT_FILENO, &max_col, NULL);
		output_func(&mode, display_all);
		return EXIT_SUCCESS;
	}

	/* Second pass: perform actions */
	k = 0;
	while (argv[++k]) {
		const struct mode_info *mp;
		const struct control_info *cp;
		const char *arg = argv[k];
		const char *argnext = argv[k+1];
		int param;

		if (arg[0] == '-') {
			mp = find_mode(arg+1);
			if (mp) {
				set_mode(mp, 1 /* reversed */, &mode);
				stty_state |= STTY_require_set_attr;
			}
			/* It is an option - already parsed. Skip it */
			continue;
		}

		mp = find_mode(arg);
		if (mp) {
			set_mode(mp, 0 /* non-reversed */, &mode);
			stty_state |= STTY_require_set_attr;
			continue;
		}

		cp = find_control(arg);
		if (cp) {
			++k;
			set_control_char_or_die(cp, argnext, &mode);
			stty_state |= STTY_require_set_attr;
			continue;
		}

		param = find_param(arg);
		if (param & param_need_arg) {
			++k;
		}

		switch (param) {
#ifdef HAVE_C_LINE
		case param_line:
			mode.c_line = xatoul_sfx(argnext, stty_suffixes);
			stty_state |= STTY_require_set_attr;
			break;
#endif
#ifdef TIOCGWINSZ
		case param_cols:
			set_window_size(-1, xatoul_sfx(argnext, stty_suffixes));
			break;
		case param_size:
			display_window_size(0);
			break;
		case param_rows:
			set_window_size(xatoul_sfx(argnext, stty_suffixes), -1);
			break;
#endif
		case param_speed:
			display_speed(&mode, 0);
			break;
		case param_ispeed:
			set_speed_or_die(input_speed, argnext, &mode);
			stty_state |= (STTY_require_set_attr | STTY_speed_was_set);
			break;
		case param_ospeed:
			set_speed_or_die(output_speed, argnext, &mode);
			stty_state |= (STTY_require_set_attr | STTY_speed_was_set);
			break;
		default:
			if (recover_mode(arg, &mode) == 1)
				stty_state |= STTY_require_set_attr;
			else /* true: if (tty_value_to_baud(xatou(arg)) != (speed_t) -1) */{
				set_speed_or_die(both_speeds, arg, &mode);
				stty_state |= (STTY_require_set_attr | STTY_speed_was_set);
			} /* else - impossible (caught in the first pass):
				bb_error_msg_and_die("invalid argument '%s'", arg); */
		}
	}

	if (stty_state & STTY_require_set_attr) {
		struct termios new_mode;

		if (tcsetattr(STDIN_FILENO, TCSADRAIN, &mode))
			perror_on_device_and_die("%s");

		/* POSIX (according to Zlotnick's book) tcsetattr returns zero if
		   it performs *any* of the requested operations.  This means it
		   can report 'success' when it has actually failed to perform
		   some proper subset of the requested operations.  To detect
		   this partial failure, get the current terminal attributes and
		   compare them to the requested ones */

		/* Initialize to all zeroes so there is no risk memcmp will report a
		   spurious difference in an uninitialized portion of the structure */
		memset(&new_mode, 0, sizeof(new_mode));
		if (tcgetattr(STDIN_FILENO, &new_mode))
			perror_on_device_and_die("%s");

		if (memcmp(&mode, &new_mode, sizeof(mode)) != 0) {
#ifdef CIBAUD
			/* SunOS 4.1.3 (at least) has the problem that after this sequence,
			   tcgetattr (&m1); tcsetattr (&m1); tcgetattr (&m2);
			   sometimes (m1 != m2).  The only difference is in the four bits
			   of the c_cflag field corresponding to the baud rate.  To save
			   Sun users a little confusion, don't report an error if this
			   happens.  But suppress the error only if we haven't tried to
			   set the baud rate explicitly -- otherwise we'd never give an
			   error for a true failure to set the baud rate */

			new_mode.c_cflag &= (~CIBAUD);
			if ((stty_state & STTY_speed_was_set)
			 || memcmp(&mode, &new_mode, sizeof(mode)) != 0)
#endif
				perror_on_device_and_die("%s: cannot perform all requested operations");
		}
	}

	return EXIT_SUCCESS;
}
