/* vi: set sw=4 ts=4: */
/*
 * test implementation for busybox
 *
 * Copyright (c) by a whole pile of folks:
 *
 *     test(1); version 7-like  --  author Erik Baalbergen
 *     modified by Eric Gisin to be used as built-in.
 *     modified by Arnold Robbins to add SVR3 compatibility
 *     (-x -c -b -p -u -g -k) plus Korn's -L -nt -ot -ef and new -S (socket).
 *     modified by J.T. Conklin for NetBSD.
 *     modified by Herbert Xu to be used as built-in in ash.
 *     modified by Erik Andersen <andersen@codepoet.org> to be used
 *     in busybox.
 *     modified by Bernhard Reutner-Fischer to be useable (i.e. a bit less bloaty).
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 *
 * Original copyright notice states:
 *     "This program is in the Public Domain."
 */

#include "libbb.h"
#include <setjmp.h>

/* This is a NOFORK applet. Be very careful! */

/* test_main() is called from shells, and we need to be extra careful here.
 * This is true regardless of PREFER_APPLETS and STANDALONE_SHELL
 * state. */


/* test(1) accepts the following grammar:
	oexpr	::= aexpr | aexpr "-o" oexpr ;
	aexpr	::= nexpr | nexpr "-a" aexpr ;
	nexpr	::= primary | "!" primary
	primary ::= unary-operator operand
		| operand binary-operator operand
		| operand
		| "(" oexpr ")"
		;
	unary-operator ::= "-r"|"-w"|"-x"|"-f"|"-d"|"-c"|"-b"|"-p"|
		"-u"|"-g"|"-k"|"-s"|"-t"|"-z"|"-n"|"-o"|"-O"|"-G"|"-L"|"-S";

	binary-operator ::= "="|"=="|"!="|"-eq"|"-ne"|"-ge"|"-gt"|"-le"|"-lt"|
			"-nt"|"-ot"|"-ef";
	operand ::= <any legal UNIX file name>
*/

#define TEST_DEBUG 0

enum token {
	EOI,
	FILRD,
	FILWR,
	FILEX,
	FILEXIST,
	FILREG,
	FILDIR,
	FILCDEV,
	FILBDEV,
	FILFIFO,
	FILSOCK,
	FILSYM,
	FILGZ,
	FILTT,
	FILSUID,
	FILSGID,
	FILSTCK,
	FILNT,
	FILOT,
	FILEQ,
	FILUID,
	FILGID,
	STREZ,
	STRNZ,
	STREQ,
	STRNE,
	STRLT,
	STRGT,
	INTEQ,
	INTNE,
	INTGE,
	INTGT,
	INTLE,
	INTLT,
	UNOT,
	BAND,
	BOR,
	LPAREN,
	RPAREN,
	OPERAND
};
#define is_int_op(a)      (((unsigned char)((a) - INTEQ)) <= 5)
#define is_str_op(a)      (((unsigned char)((a) - STREZ)) <= 5)
#define is_file_op(a)     (((unsigned char)((a) - FILNT)) <= 2)
#define is_file_access(a) (((unsigned char)((a) - FILRD)) <= 2)
#define is_file_type(a)   (((unsigned char)((a) - FILREG)) <= 5)
#define is_file_bit(a)    (((unsigned char)((a) - FILSUID)) <= 2)

#if TEST_DEBUG
int depth;
#define nest_msg(...) do { \
	depth++; \
	fprintf(stderr, "%*s", depth*2, ""); \
	fprintf(stderr, __VA_ARGS__); \
} while (0)
#define unnest_msg(...) do { \
	fprintf(stderr, "%*s", depth*2, ""); \
	fprintf(stderr, __VA_ARGS__); \
	depth--; \
} while (0)
#define dbg_msg(...) do { \
	fprintf(stderr, "%*s", depth*2, ""); \
	fprintf(stderr, __VA_ARGS__); \
} while (0)
#define unnest_msg_and_return(expr, ...) do { \
	number_t __res = (expr); \
	fprintf(stderr, "%*s", depth*2, ""); \
	fprintf(stderr, __VA_ARGS__, res); \
	depth--; \
	return __res; \
} while (0)
static const char *const TOKSTR[] = {
	"EOI",
	"FILRD",
	"FILWR",
	"FILEX",
	"FILEXIST",
	"FILREG",
	"FILDIR",
	"FILCDEV",
	"FILBDEV",
	"FILFIFO",
	"FILSOCK",
	"FILSYM",
	"FILGZ",
	"FILTT",
	"FILSUID",
	"FILSGID",
	"FILSTCK",
	"FILNT",
	"FILOT",
	"FILEQ",
	"FILUID",
	"FILGID",
	"STREZ",
	"STRNZ",
	"STREQ",
	"STRNE",
	"STRLT",
	"STRGT",
	"INTEQ",
	"INTNE",
	"INTGE",
	"INTGT",
	"INTLE",
	"INTLT",
	"UNOT",
	"BAND",
	"BOR",
	"LPAREN",
	"RPAREN",
	"OPERAND"
};
#else
#define nest_msg(...)   ((void)0)
#define unnest_msg(...) ((void)0)
#define dbg_msg(...)    ((void)0)
#define unnest_msg_and_return(expr, ...) return expr
#endif

enum token_types {
	UNOP,
	BINOP,
	BUNOP,
	BBINOP,
	PAREN
};

struct operator_t {
	char op_text[4];
	unsigned char op_num, op_type;
};

static const struct operator_t ops[] = {
	{ "-r", FILRD   , UNOP   },
	{ "-w", FILWR   , UNOP   },
	{ "-x", FILEX   , UNOP   },
	{ "-e", FILEXIST, UNOP   },
	{ "-f", FILREG  , UNOP   },
	{ "-d", FILDIR  , UNOP   },
	{ "-c", FILCDEV , UNOP   },
	{ "-b", FILBDEV , UNOP   },
	{ "-p", FILFIFO , UNOP   },
	{ "-u", FILSUID , UNOP   },
	{ "-g", FILSGID , UNOP   },
	{ "-k", FILSTCK , UNOP   },
	{ "-s", FILGZ   , UNOP   },
	{ "-t", FILTT   , UNOP   },
	{ "-z", STREZ   , UNOP   },
	{ "-n", STRNZ   , UNOP   },
	{ "-h", FILSYM  , UNOP   },    /* for backwards compat */

	{ "-O" , FILUID , UNOP   },
	{ "-G" , FILGID , UNOP   },
	{ "-L" , FILSYM , UNOP   },
	{ "-S" , FILSOCK, UNOP   },
	{ "="  , STREQ  , BINOP  },
	{ "==" , STREQ  , BINOP  },
	{ "!=" , STRNE  , BINOP  },
	{ "<"  , STRLT  , BINOP  },
	{ ">"  , STRGT  , BINOP  },
	{ "-eq", INTEQ  , BINOP  },
	{ "-ne", INTNE  , BINOP  },
	{ "-ge", INTGE  , BINOP  },
	{ "-gt", INTGT  , BINOP  },
	{ "-le", INTLE  , BINOP  },
	{ "-lt", INTLT  , BINOP  },
	{ "-nt", FILNT  , BINOP  },
	{ "-ot", FILOT  , BINOP  },
	{ "-ef", FILEQ  , BINOP  },
	{ "!"  , UNOT   , BUNOP  },
	{ "-a" , BAND   , BBINOP },
	{ "-o" , BOR    , BBINOP },
	{ "("  , LPAREN , PAREN  },
	{ ")"  , RPAREN , PAREN  },
};


#if ENABLE_FEATURE_TEST_64
typedef int64_t number_t;
#else
typedef int number_t;
#endif


/* We try to minimize both static and stack usage. */
struct test_statics {
	char **args;
	/* set only by check_operator(), either to bogus struct
	 * or points to matching operator_t struct. Never NULL. */
	const struct operator_t *last_operator;
	gid_t *group_array;
	int ngroups;
	jmp_buf leaving;
};

/* See test_ptr_hack.c */
extern struct test_statics *const test_ptr_to_statics;

#define S (*test_ptr_to_statics)
#define args            (S.args         )
#define last_operator   (S.last_operator)
#define group_array     (S.group_array  )
#define ngroups         (S.ngroups      )
#define leaving         (S.leaving      )

#define INIT_S() do { \
	(*(struct test_statics**)&test_ptr_to_statics) = xzalloc(sizeof(S)); \
	barrier(); \
} while (0)
#define DEINIT_S() do { \
	free(test_ptr_to_statics); \
} while (0)

static number_t primary(enum token n);

static void syntax(const char *op, const char *msg) NORETURN;
static void syntax(const char *op, const char *msg)
{
	if (op && *op) {
		bb_error_msg("%s: %s", op, msg);
	} else {
		bb_error_msg("%s: %s"+4, msg);
	}
	longjmp(leaving, 2);
}

/* atoi with error detection */
//XXX: FIXME: duplicate of existing libbb function?
static number_t getn(const char *s)
{
	char *p;
#if ENABLE_FEATURE_TEST_64
	long long r;
#else
	long r;
#endif

	errno = 0;
#if ENABLE_FEATURE_TEST_64
	r = strtoll(s, &p, 10);
#else
	r = strtol(s, &p, 10);
#endif

	if (errno != 0)
		syntax(s, "out of range");

	if (*(skip_whitespace(p)))
		syntax(s, "bad number");

	return r;
}

/* UNUSED
static int newerf(const char *f1, const char *f2)
{
	struct stat b1, b2;

	return (stat(f1, &b1) == 0 &&
			stat(f2, &b2) == 0 && b1.st_mtime > b2.st_mtime);
}

static int olderf(const char *f1, const char *f2)
{
	struct stat b1, b2;

	return (stat(f1, &b1) == 0 &&
			stat(f2, &b2) == 0 && b1.st_mtime < b2.st_mtime);
}

static int equalf(const char *f1, const char *f2)
{
	struct stat b1, b2;

	return (stat(f1, &b1) == 0 &&
			stat(f2, &b2) == 0 &&
			b1.st_dev == b2.st_dev && b1.st_ino == b2.st_ino);
}
*/


static enum token check_operator(char *s)
{
	static const struct operator_t no_op = {
		.op_num = -1,
		.op_type = -1
	};
	const struct operator_t *op;

	last_operator = &no_op;
	if (s == NULL) {
		return EOI;
	}

	op = ops;
	do {
		if (strcmp(s, op->op_text) == 0) {
			last_operator = op;
			return op->op_num;
		}
		op++;
	} while (op < ops + ARRAY_SIZE(ops));

	return OPERAND;
}


static int binop(void)
{
	const char *opnd1, *opnd2;
	const struct operator_t *op;
	number_t val1, val2;

	opnd1 = *args;
	check_operator(*++args);
	op = last_operator;

	opnd2 = *++args;
	if (opnd2 == NULL)
		syntax(op->op_text, "argument expected");

	if (is_int_op(op->op_num)) {
		val1 = getn(opnd1);
		val2 = getn(opnd2);
		if (op->op_num == INTEQ)
			return val1 == val2;
		if (op->op_num == INTNE)
			return val1 != val2;
		if (op->op_num == INTGE)
			return val1 >= val2;
		if (op->op_num == INTGT)
			return val1 >  val2;
		if (op->op_num == INTLE)
			return val1 <= val2;
		if (op->op_num == INTLT)
			return val1 <  val2;
	}
	if (is_str_op(op->op_num)) {
		val1 = strcmp(opnd1, opnd2);
		if (op->op_num == STREQ)
			return val1 == 0;
		if (op->op_num == STRNE)
			return val1 != 0;
		if (op->op_num == STRLT)
			return val1 < 0;
		if (op->op_num == STRGT)
			return val1 > 0;
	}
	/* We are sure that these three are by now the only binops we didn't check
	 * yet, so we do not check if the class is correct:
	 */
/*	if (is_file_op(op->op_num)) */
	{
		struct stat b1, b2;

		if (stat(opnd1, &b1) || stat(opnd2, &b2))
			return 0; /* false, since at least one stat failed */
		if (op->op_num == FILNT)
			return b1.st_mtime > b2.st_mtime;
		if (op->op_num == FILOT)
			return b1.st_mtime < b2.st_mtime;
		if (op->op_num == FILEQ)
			return b1.st_dev == b2.st_dev && b1.st_ino == b2.st_ino;
	}
	return 1; /* NOTREACHED */
}


static void initialize_group_array(void)
{
	ngroups = getgroups(0, NULL);
	if (ngroups > 0) {
		/* FIXME: ash tries so hard to not die on OOM,
		 * and we spoil it with just one xrealloc here */
		/* We realloc, because test_main can be entered repeatedly by shell.
		 * Testcase (ash): 'while true; do test -x some_file; done'
		 * and watch top. (some_file must have owner != you) */
		group_array = xrealloc(group_array, ngroups * sizeof(gid_t));
		getgroups(ngroups, group_array);
	}
}


/* Return non-zero if GID is one that we have in our groups list. */
//XXX: FIXME: duplicate of existing libbb function?
// see toplevel TODO file:
// possible code duplication ingroup() and is_a_group_member()
static int is_a_group_member(gid_t gid)
{
	int i;

	/* Short-circuit if possible, maybe saving a call to getgroups(). */
	if (gid == getgid() || gid == getegid())
		return 1;

	if (ngroups == 0)
		initialize_group_array();

	/* Search through the list looking for GID. */
	for (i = 0; i < ngroups; i++)
		if (gid == group_array[i])
			return 1;

	return 0;
}


/* Do the same thing access(2) does, but use the effective uid and gid,
   and don't make the mistake of telling root that any file is
   executable. */
static int test_eaccess(char *path, int mode)
{
	struct stat st;
	unsigned int euid = geteuid();

	if (stat(path, &st) < 0)
		return -1;

	if (euid == 0) {
		/* Root can read or write any file. */
		if (mode != X_OK)
			return 0;

		/* Root can execute any file that has any one of the execute
		   bits set. */
		if (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
			return 0;
	}

	if (st.st_uid == euid)  /* owner */
		mode <<= 6;
	else if (is_a_group_member(st.st_gid))
		mode <<= 3;

	if (st.st_mode & mode)
		return 0;

	return -1;
}


static int filstat(char *nm, enum token mode)
{
	struct stat s;
	unsigned i = i; /* gcc 3.x thinks it can be used uninitialized */

	if (mode == FILSYM) {
#ifdef S_IFLNK
		if (lstat(nm, &s) == 0) {
			i = S_IFLNK;
			goto filetype;
		}
#endif
		return 0;
	}

	if (stat(nm, &s) != 0)
		return 0;
	if (mode == FILEXIST)
		return 1;
	if (is_file_access(mode)) {
		if (mode == FILRD)
			i = R_OK;
		if (mode == FILWR)
			i = W_OK;
		if (mode == FILEX)
			i = X_OK;
		return test_eaccess(nm, i) == 0;
	}
	if (is_file_type(mode)) {
		if (mode == FILREG)
			i = S_IFREG;
		if (mode == FILDIR)
			i = S_IFDIR;
		if (mode == FILCDEV)
			i = S_IFCHR;
		if (mode == FILBDEV)
			i = S_IFBLK;
		if (mode == FILFIFO) {
#ifdef S_IFIFO
			i = S_IFIFO;
#else
			return 0;
#endif
		}
		if (mode == FILSOCK) {
#ifdef S_IFSOCK
			i = S_IFSOCK;
#else
			return 0;
#endif
		}
 filetype:
		return ((s.st_mode & S_IFMT) == i);
	}
	if (is_file_bit(mode)) {
		if (mode == FILSUID)
			i = S_ISUID;
		if (mode == FILSGID)
			i = S_ISGID;
		if (mode == FILSTCK)
			i = S_ISVTX;
		return ((s.st_mode & i) != 0);
	}
	if (mode == FILGZ)
		return s.st_size > 0L;
	if (mode == FILUID)
		return s.st_uid == geteuid();
	if (mode == FILGID)
		return s.st_gid == getegid();
	return 1; /* NOTREACHED */
}


static number_t nexpr(enum token n)
{
	number_t res;

	nest_msg(">nexpr(%s)\n", TOKSTR[n]);
	if (n == UNOT) {
		res = !nexpr(check_operator(*++args));
		unnest_msg("<nexpr:%lld\n", res);
		return res;
	}
	res = primary(n);
	unnest_msg("<nexpr:%lld\n", res);
	return res;
}


static number_t aexpr(enum token n)
{
	number_t res;

	nest_msg(">aexpr(%s)\n", TOKSTR[n]);
	res = nexpr(n);
	dbg_msg("aexpr: nexpr:%lld, next args:%s\n", res, args[1]);
	if (check_operator(*++args) == BAND) {
		dbg_msg("aexpr: arg is AND, next args:%s\n", args[1]);
		res = aexpr(check_operator(*++args)) && res;
		unnest_msg("<aexpr:%lld\n", res);
		return res;
	}
	args--;
	unnest_msg("<aexpr:%lld, args:%s\n", res, args[0]);
	return res;
}


static number_t oexpr(enum token n)
{
	number_t res;

	nest_msg(">oexpr(%s)\n", TOKSTR[n]);
	res = aexpr(n);
	dbg_msg("oexpr: aexpr:%lld, next args:%s\n", res, args[1]);
	if (check_operator(*++args) == BOR) {
		dbg_msg("oexpr: next arg is OR, next args:%s\n", args[1]);
		res = oexpr(check_operator(*++args)) || res;
		unnest_msg("<oexpr:%lld\n", res);
		return res;
	}
	args--;
	unnest_msg("<oexpr:%lld, args:%s\n", res, args[0]);
	return res;
}


static number_t primary(enum token n)
{
#if TEST_DEBUG
	number_t res = res; /* for compiler */
#else
	number_t res;
#endif
	const struct operator_t *args0_op;

	nest_msg(">primary(%s)\n", TOKSTR[n]);
	if (n == EOI) {
		syntax(NULL, "argument expected");
	}
	if (n == LPAREN) {
		res = oexpr(check_operator(*++args));
		if (check_operator(*++args) != RPAREN)
			syntax(NULL, "closing paren expected");
		unnest_msg("<primary:%lld\n", res);
		return res;
	}

	/* coreutils 6.9 checks "is args[1] binop and args[2] exist?" first,
	 * do the same */
	args0_op = last_operator;
	/* last_operator = operator at args[1] */
	if (check_operator(args[1]) != EOI) { /* if args[1] != NULL */
		if (args[2]) {
			// coreutils also does this:
			// if (args[3] && args[0]="-l" && args[2] is BINOP)
			//	return binop(1 /* prepended by -l */);
			if (last_operator->op_type == BINOP)
				unnest_msg_and_return(binop(), "<primary: binop:%lld\n");
		}
	}
	/* check "is args[0] unop?" second */
	if (args0_op->op_type == UNOP) {
		/* unary expression */
		if (args[1] == NULL)
//			syntax(args0_op->op_text, "argument expected");
			goto check_emptiness;
		args++;
		if (n == STREZ)
			unnest_msg_and_return(args[0][0] == '\0', "<primary:%lld\n");
		if (n == STRNZ)
			unnest_msg_and_return(args[0][0] != '\0', "<primary:%lld\n");
		if (n == FILTT)
			unnest_msg_and_return(isatty(getn(*args)), "<primary: isatty(%s)%lld\n", *args);
		unnest_msg_and_return(filstat(*args, n), "<primary: filstat(%s):%lld\n", *args);
	}

	/*check_operator(args[1]); - already done */
	if (last_operator->op_type == BINOP) {
		/* args[2] is known to be NULL, isn't it bound to fail? */
		unnest_msg_and_return(binop(), "<primary:%lld\n");
	}
 check_emptiness:
	unnest_msg_and_return(args[0][0] != '\0', "<primary:%lld\n");
}


int test_main(int argc, char **argv)
{
	int res;
	const char *arg0;
//	bool negate = 0;

	arg0 = bb_basename(argv[0]);
	if (arg0[0] == '[') {
		--argc;
		if (!arg0[1]) { /* "[" ? */
			if (NOT_LONE_CHAR(argv[argc], ']')) {
				bb_error_msg("missing ]");
				return 2;
			}
		} else { /* assuming "[[" */
			if (strcmp(argv[argc], "]]") != 0) {
				bb_error_msg("missing ]]");
				return 2;
			}
		}
		argv[argc] = NULL;
	}

	/* We must do DEINIT_S() prior to returning */
	INIT_S();

	res = setjmp(leaving);
	if (res)
		goto ret;

	/* resetting ngroups is probably unnecessary.  it will
	 * force a new call to getgroups(), which prevents using
	 * group data fetched during a previous call.  but the
	 * only way the group data could be stale is if there's
	 * been an intervening call to setgroups(), and this
	 * isn't likely in the case of a shell.  paranoia
	 * prevails...
	 */
	ngroups = 0;

	//argc--;
	argv++;

	/* Implement special cases from POSIX.2, section 4.62.4 */
	if (!argv[0]) { /* "test" */
		res = 1;
		goto ret;
	}
#if 0
// Now it's fixed in the parser and should not be needed
	if (LONE_CHAR(argv[0], '!') && argv[1]) {
		negate = 1;
		//argc--;
		argv++;
	}
	if (!argv[1]) { /* "test [!] arg" */
		res = (*argv[0] == '\0');
		goto ret;
	}
	if (argv[2] && !argv[3]) {
		check_operator(argv[1]);
		if (last_operator->op_type == BINOP) {
			/* "test [!] arg1 <binary_op> arg2" */
			args = &argv[0];
			res = (binop() == 0);
			goto ret;
		}
	}

	/* Some complex expression. Undo '!' removal */
	if (negate) {
		negate = 0;
		//argc++;
		argv--;
	}
#endif
	args = &argv[0];
	res = !oexpr(check_operator(*args));

	if (*args != NULL && *++args != NULL) {
		/* TODO: example when this happens? */
		bb_error_msg("%s: unknown operand", *args);
		res = 2;
	}
 ret:
	DEINIT_S();
//	return negate ? !res : res;
	return res;
}
