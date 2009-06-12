/*
 * ldconfig - update shared library symlinks
 *
 * usage: ldconfig [-DvqnNX] [-f conf] [-C cache] [-r root] dir ...
 *        ldconfig -l [-Dv] lib ...
 *        ldconfig -p
 *        -D: debug mode, don't update links
 *        -v: verbose mode, print things as we go
 *        -q: quiet mode, don't print warnings
 *        -n: don't process standard directories
 *        -N: don't update the library cache
 *        -X: don't update the library links
 *        -l: library mode, manually link libraries
 *        -p: print the current library cache
 *        -f conf: use conf instead of /etc/ld.so.conf
 *        -C cache: use cache instead of /etc/ld.so.cache
 *        -r root: first, do a chroot to the indicated directory
 *        dir ...: directories to process
 *        lib ...: libraries to link
 *
 * Copyright 1994-2000 David Engel and Mitch D'Souza
 *
 * This program may be used for any purpose as long as this
 * copyright notice is kept.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <dirent.h>
#include <unistd.h>
#include <link.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <ld_elf.h>
#include "readsoname.h"

struct exec
{
  unsigned long a_info;		/* Use macros N_MAGIC, etc for access */
  unsigned a_text;		/* length of text, in bytes */
  unsigned a_data;		/* length of data, in bytes */
  unsigned a_bss;		/* length of uninitialized data area for file, in bytes */
  unsigned a_syms;		/* length of symbol table data in file, in bytes */
  unsigned a_entry;		/* start address */
  unsigned a_trsize;		/* length of relocation info for text, in bytes */
  unsigned a_drsize;		/* length of relocation info for data, in bytes */
};

#if !defined (N_MAGIC)
#define N_MAGIC(exec) ((exec).a_info & 0xffff)
#endif
/* Code indicating object file or impure executable.  */
#define OMAGIC 0407
/* Code indicating pure executable.  */
#define NMAGIC 0410
/* Code indicating demand-paged executable.  */
#define ZMAGIC 0413
/* This indicates a demand-paged executable with the header in the text. 
   The first page is unmapped to help trap NULL pointer references */
#define QMAGIC 0314
/* Code indicating core file.  */
#define CMAGIC 0421

char *___strtok = NULL;

/* For SunOS */
#ifndef PATH_MAX
#include <limits.h>
#define PATH_MAX _POSIX_PATH_MAX
#endif

/* For SunOS */
#ifndef N_MAGIC
#define N_MAGIC(exec) ((exec).a_magic & 0xffff)
#endif

#define EXIT_OK    0
#define EXIT_FATAL 128

char *prog = NULL;
int debug = 0;			/* debug mode */
int verbose = 0;		/* verbose mode */
int libmode = 0;		/* library mode */
int nocache = 0;		/* don't build cache */
int nolinks = 0;		/* don't update links */

char *conffile = LDSO_CONF;	/* default conf file */
char *cachefile = LDSO_CACHE;	/* default cache file */
void cache_print(void);
void cache_dolib(const char *dir, const char *so, int libtype);
void cache_write(void);

/* These two are used internally -- you shouldn't need to use them */
static void verror_msg(const char *s, va_list p)
{
	fflush(stdout);
	fprintf(stderr, "%s: ", prog);
	vfprintf(stderr, s, p);
}

extern void warnx(const char *s, ...)
{
	va_list p;

	va_start(p, s);
	verror_msg(s, p);
	va_end(p);
	fprintf(stderr, "\n");
}

extern void err(int errnum, const char *s, ...)
{
	va_list p;

	va_start(p, s);
	verror_msg(s, p);
	va_end(p);
	fprintf(stderr, "\n");
	exit(errnum);
}

static void vperror_msg(const char *s, va_list p)
{
	int err = errno;

	if (s == 0)
		s = "";
	verror_msg(s, p);
	if (*s)
		s = ": ";
	fprintf(stderr, "%s%s\n", s, strerror(err));
}

extern void warn(const char *s, ...)
{
	va_list p;

	va_start(p, s);
	vperror_msg(s, p);
	va_end(p);
}

void *xmalloc(size_t size)
{
    void *ptr;
    if ((ptr = malloc(size)) == NULL)
	err(EXIT_FATAL,"out of memory");
    return ptr;
}

char *xstrdup(const char *str)
{
    char *ptr;
    if ((ptr = strdup(str)) == NULL)
	err(EXIT_FATAL,"out of memory");
    return ptr;
}

/* If shared library, return a malloced copy of the soname and set the
   type, else return NULL.

   expected_type should be either LIB_ANY or one of the following:-
   LIB_DLL
   LIB_ELF
   LIB_ELF_LIBC5
   LIB_ELF_LIBC6

   If the lib is ELF and we can not deduce the type the type will
   be set based on expected_type.

   If the expected, actual/deduced types missmatch we display a warning
   and use the actual/deduced type.
*/
char *is_shlib(const char *dir, const char *name, int *type,
	int *islink, int expected_type)
{
    char *good = NULL;
    char *cp, *cp2;
    FILE *file;
    struct exec exec;
    ElfW(Ehdr) *elf_hdr;
    struct stat statbuf;
    char buff[4096];

    /* see if name is of the form *.so* */
    if (name[strlen(name)-1] != '~' && (cp = strstr(name, ".so")))
    {
	/* find the start of the Vminor part, if any */
	if (cp[3] == '.' && (cp2 = strchr(cp + 4, '.')))
	    cp = cp2;
	else
	    cp = cp + strlen(cp);

	/* construct the full path name */
	sprintf(buff, "%s%s%s", dir, (*dir && strcmp(dir, "/")) ?
		"/" : "", name);

	/* first, make sure it's a regular file */
	if (lstat(buff, &statbuf))
	    warn("skipping %s", buff);
	else if (!S_ISREG(statbuf.st_mode) && !S_ISLNK(statbuf.st_mode))
	    warnx("%s is not a regular file or symlink, skipping", buff);
	else
	{
	    /* is it a regular file or a symlink */
	    *islink = S_ISLNK(statbuf.st_mode);

	    /* then try opening it */
	    if (!(file = fopen(buff, "rb")))
		warn("skipping %s", buff);
	    else
	    {
		/* now make sure it's a shared library */
		if (fread(&exec, sizeof exec, 1, file) < 1)
		    warnx("can't read header from %s, skipping", buff);
		else if (N_MAGIC(exec) != ZMAGIC && N_MAGIC(exec) != QMAGIC)
		{
		    elf_hdr = (ElfW(Ehdr) *) &exec;
		    if (elf_hdr->e_ident[0] != 0x7f ||
			    strncmp(&elf_hdr->e_ident[1], "ELF",3) != 0)
		    {
			/* silently ignore linker scripts */
			if (strncmp((char *)&exec, "/* GNU ld", 9) != 0)
			    warnx("%s is not a shared library, skipping", buff);
		    }
		    else
		    {
			/* always call readsoname to update type */
			if(expected_type == LIB_DLL) {
			    warnx("%s is not an a.out library, its ELF!\n", buff);
			    expected_type=LIB_ANY;
			}
			*type = LIB_ELF;
			good = readsoname(buff, file, expected_type, type, 
				elf_hdr->e_ident[EI_CLASS]);
			if (good == NULL || *islink)
			{
			    if (good != NULL)
				free(good);
			    good = xstrdup(name);
			}
			else
			{
			    /* if the soname does not match the filename,
			       issue a warning, but only in debug mode. */
			    int len = strlen(good);
			    if (debug && (strncmp(good, name, len) != 0 ||
					(name[len] != '\0' && name[len] != '.')))
				warnx("%s has inconsistent soname (%s)", buff, good);
			}
		    }
		}
		else
		{
		    if (*islink)
			good = xstrdup(name);
		    else
		    {
			good = xmalloc(cp - name + 1);
			strncpy(good, name, cp - name);
			good[cp - name] = '\0';
		    }
		    if(expected_type != LIB_ANY && expected_type != LIB_DLL)
		    {
			warnx("%s is not an ELF library, its an a.out DLL!", buff);
			expected_type=LIB_ANY;
		    }

		    *type = LIB_DLL;
		}
		fclose(file);
	    }
	}
    }

    return good;
}

/* update the symlink to new library */
void link_shlib(const char *dir, const char *file, const char *so)
{
    int change = 1;
    char libname[4096];
    char linkname[4096];
    struct stat libstat;
    struct stat linkstat;

    /* construct the full path names */
    sprintf(libname, "%s/%s", dir, file);
    sprintf(linkname, "%s/%s", dir, so);

    /* see if a link already exists */
    if (!stat(linkname, &linkstat))
    {
	/* now see if it's the one we want */
	if (stat(libname, &libstat))
	    warn("can't stat %s", libname);
	else if (libstat.st_dev == linkstat.st_dev &&
		libstat.st_ino == linkstat.st_ino)
	    change = 0;
    }

    /* then update the link, if required */
    if (change > 0 && !nolinks)
    {
	if (!lstat(linkname, &linkstat))
	{
	    if (!S_ISLNK(linkstat.st_mode))
	    {
		warnx("%s is not a symlink", linkname);
		change = -1;
	    }
	    else if (remove(linkname))
	    {
		warn("can't unlink %s", linkname);
		change = -1;
	    }
	}
	if (change > 0)
	{
	    if (symlink(file, linkname))
	    {
		warn("can't link %s to %s", linkname, file);
		change = -1;
	    }
	}
    }

    /* some people like to know what we're doing */
    if (verbose > 0)
	printf("\t%s => %s%s\n", so, file,
		change < 0 ? " (SKIPPED)" :
		(change > 0 ? " (changed)" : ""));

    return;
}

/* figure out which library is greater */
int libcmp(char *p1, char *p2)
{
    while (*p1)
    {
	if (isdigit(*p1) && isdigit(*p2))
	{
	    /* must compare this numerically */
	    int v1, v2;
	    v1 = strtoul(p1, &p1, 10);
	    v2 = strtoul(p2, &p2, 10);
	    if (v1 != v2)
		return v1 - v2;
	}
	else if (isdigit(*p1) && !isdigit(*p2))
	    return 1;
	else if (!isdigit(*p1) && isdigit(*p2))
	    return -1;
	else if (*p1 != *p2)
	    return *p1 - *p2;
	else
	    p1++, p2++;
    }

    return *p1 - *p2;
}

struct lib
{
    char *so;			/* soname of a library */
    char *name;			/* name of a library */
    int libtype;		/* type of a library */
    int islink;			/* is it a symlink */
    struct lib *next;		/* next library in list */
};

/* update all shared library links in a directory */
void scan_dir(const char *rawname)
{
    DIR *dir;
    const char *name;
    struct dirent *ent;
    char *so, *path, *path_n;
    struct lib *lp, *libs = NULL;
    int i, libtype, islink, expected_type = LIB_ANY;

    /* We need a writable copy of this string */
    path = strdup(rawname);
    if (!path) {
	err(EXIT_FATAL, "Out of memory!\n");
    }
    /* Eliminate all double //s */
    path_n=path;
    while((path_n=strstr(path_n, "//"))) {
	i = strlen(path_n);
	memmove(path_n, path_n+1, i-1);
	*(path_n + i - 1)='\0';
    }
    name = path;

#if 0
    char *t;
    /* Check for an embedded expected type */
    t=strrchr(name, '=');
    if( t )
    {
	*t++ = '\0'; /* Skip = char */
	if(strcasecmp(t, "libc4") == 0)
	{
	    expected_type = LIB_DLL;
	} 
	else
	{
	    if(strcasecmp(t, "libc5") == 0)
	    {
		expected_type = LIB_ELF_LIBC5; 
	    }
	    else
	    {
		if(strcasecmp(t, "libc6") == 0)
		{
		    expected_type = LIB_ELF_LIBC6;
		}
		else 
		{
		    if(strcasecmp(t, "libc0") == 0)
		    {
			expected_type = LIB_ELF_LIBC0;
		    }
		    else
		    {
			warnx("Unknown type field '%s' for dir '%s' - ignored\n", t, name);
			expected_type = LIB_ANY;
		    }
		}
	    }
	}
    }
#endif

    /* let 'em know what's going on */
    if (verbose > 0)
	printf("%s:\n", name);

    /* if we can't open it, we can't do anything */
    if ((dir = opendir(name)) == NULL)
    {
	warn("skipping %s", name);
	free(path);
	return;
    }

    /* yes, we have to look at every single file */
    while ((ent = readdir(dir)) != NULL)
    {
	/* if it's not a shared library, don't bother */
	if ((so = is_shlib(name, ent->d_name, &libtype, &islink, expected_type)) == NULL)
	    continue;

	/* have we already seen one with the same so name? */
	for (lp = libs; lp; lp = lp->next)
	{
	    if (strcmp(so, lp->so) == 0)
	    {
		/* we have, which one do we want to use? */
		if ((!islink && lp->islink) ||
			(islink == lp->islink && 
			 libcmp(ent->d_name, lp->name) > 0))
		{
		    /* let's use the new one */
		    free(lp->name);
		    lp->name = xstrdup(ent->d_name);
		    lp->libtype = libtype;
		    lp->islink = islink;
		} 
		break;
	    }
	}

	/* congratulations, you're the first one we've seen */
	if (!lp)
	{
	    lp = xmalloc(sizeof *lp);
	    lp->so = xstrdup(so);
	    lp->name = xstrdup(ent->d_name);
	    lp->libtype = libtype;
	    lp->islink = islink;
	    lp->next = libs;
	    libs = lp;
	}

	free(so);
    }

    /* don't need this any more */
    closedir(dir);

    /* now we have all the latest libs, update the links */
    for (lp = libs; lp; lp = lp->next)
    {
	if (!lp->islink)
	    link_shlib(name, lp->name, lp->so);
#ifdef USE_CACHE
	if (!nocache)
	    cache_dolib(name, lp->so, lp->libtype);
#endif
    }

    /* always try to clean up after ourselves */
    while (libs)
    {
	lp = libs->next;
	free(libs->so);
	free(libs->name);
	free(libs);
	libs = lp;
    }

    free(path);
    return;
}

/* return the list of system-specific directories */
char *get_extpath(void)
{
    char *res = NULL, *cp;
    FILE *file;
    struct stat stat;

    if ((file = fopen(conffile, "r")) != NULL)
    {
	fstat(fileno(file), &stat);
	res = xmalloc(stat.st_size + 1);
	fread(res, 1, stat.st_size, file);
	fclose(file);
	res[stat.st_size] = '\0';

	/* convert comments fo spaces */
	for (cp = res; *cp; /*nada*/) {
	    if (*cp == '#') {
		do
		    *cp++ = ' ';
		while (*cp && *cp != '\n');
	    } else {
		cp++;
	    }
	}	  
    }

    return res;
}

#ifdef USE_CACHE
typedef struct liblist
{
    int flags;
    int sooffset;
    int liboffset;
    char *soname;
    char *libname;
    struct liblist *next;
} liblist_t;

static header_t magic = { LDSO_CACHE_MAGIC, LDSO_CACHE_VER, 0 };
static liblist_t *lib_head = NULL;

static int liblistcomp(liblist_t *x, liblist_t *y)
{
    int res;

    if ((res = libcmp(x->soname, y->soname)) == 0)
    {
	res = libcmp(strrchr(x->libname, '/') + 1,
		strrchr(y->libname, '/') + 1);
    }

    return res;
}

void cache_dolib(const char *dir, const char *so, int libtype)
{
    char fullpath[PATH_MAX];
    liblist_t *new_lib, *cur_lib;

    magic.nlibs++;
    sprintf(fullpath, "%s/%s", dir, so);
    new_lib = xmalloc(sizeof (liblist_t));
    new_lib->flags = libtype;
    new_lib->soname = xstrdup(so);
    new_lib->libname = xstrdup(fullpath);

    if (lib_head == NULL || liblistcomp(new_lib, lib_head) > 0)
    {
	new_lib->next = lib_head;
	lib_head = new_lib;
    }
    else
    {
	for (cur_lib = lib_head; cur_lib->next != NULL &&
		liblistcomp(new_lib, cur_lib->next) <= 0;
		cur_lib = cur_lib->next)
	    /* nothing */;
	new_lib->next = cur_lib->next;
	cur_lib->next = new_lib;
    }
}

void cache_write(void)
{
    int cachefd;
    int stroffset = 0;
    char tempfile[4096];
    liblist_t *cur_lib;

    if (!magic.nlibs)
	return;

    sprintf(tempfile, "%s~", cachefile);

    if (unlink(tempfile) && errno != ENOENT)
	err(EXIT_FATAL,"can't unlink %s (%s)", tempfile, strerror(errno));

    if ((cachefd = creat(tempfile, 0644)) < 0)
	err(EXIT_FATAL,"can't create %s (%s)", tempfile, strerror(errno));

    if (write(cachefd, &magic, sizeof (header_t)) != sizeof (header_t))
	err(EXIT_FATAL,"can't write %s (%s)", tempfile, strerror(errno));

    for (cur_lib = lib_head; cur_lib != NULL; cur_lib = cur_lib->next)
    {
	cur_lib->sooffset = stroffset;
	stroffset += strlen(cur_lib->soname) + 1;
	cur_lib->liboffset = stroffset;
	stroffset += strlen(cur_lib->libname) + 1;
	if (write(cachefd, cur_lib, sizeof (libentry_t)) !=
		sizeof (libentry_t))
	    err(EXIT_FATAL,"can't write %s (%s)", tempfile, strerror(errno));
    }

    for (cur_lib = lib_head; cur_lib != NULL; cur_lib = cur_lib->next)
    {
	if (write(cachefd, cur_lib->soname, strlen(cur_lib->soname) + 1)
		!= strlen(cur_lib->soname) + 1)
	    err(EXIT_FATAL,"can't write %s (%s)", tempfile, strerror(errno));
	if (write(cachefd, cur_lib->libname, strlen(cur_lib->libname) + 1)
		!= strlen(cur_lib->libname) + 1)
	    err(EXIT_FATAL,"can't write %s (%s)", tempfile, strerror(errno));
    }

    if (close(cachefd))
	err(EXIT_FATAL,"can't close %s (%s)", tempfile, strerror(errno));

    if (chmod(tempfile, 0644))
	err(EXIT_FATAL,"can't chmod %s (%s)", tempfile, strerror(errno));

    if (rename(tempfile, cachefile))
	err(EXIT_FATAL,"can't rename %s (%s)", tempfile, strerror(errno));
}

void cache_print(void)
{
    caddr_t c;
    struct stat st;
    int fd = 0;
    char *strs;
    header_t *header;
    libentry_t *libent;

    if (stat(cachefile, &st) || (fd = open(cachefile, O_RDONLY))<0)
	err(EXIT_FATAL,"can't read %s (%s)", cachefile, strerror(errno));
    if ((c = mmap(0,st.st_size, PROT_READ, MAP_SHARED ,fd, 0)) == (caddr_t)-1)
	err(EXIT_FATAL,"can't map %s (%s)", cachefile, strerror(errno));
    close(fd);

    if (memcmp(((header_t *)c)->magic, LDSO_CACHE_MAGIC, LDSO_CACHE_MAGIC_LEN))
	err(EXIT_FATAL,"%s cache corrupt", cachefile);

    if (memcmp(((header_t *)c)->version, LDSO_CACHE_VER, LDSO_CACHE_VER_LEN))
	err(EXIT_FATAL,"wrong cache version - expected %s", LDSO_CACHE_VER);

    header = (header_t *)c;
    libent = (libentry_t *)(c + sizeof (header_t));
    strs = (char *)&libent[header->nlibs];

    printf("%d libs found in cache `%s' (version %s)\n",
	    header->nlibs, cachefile, LDSO_CACHE_VER);

    for (fd = 0; fd < header->nlibs; fd++)
    {
	printf("\t%s ", strs + libent[fd].sooffset);
	switch (libent[fd].flags & ~LIB_ELF64) 
	{
	    case LIB_DLL:
		printf("(libc4)");
		break;
	    case LIB_ELF:
		printf("(ELF%s)", libent[fd].flags & LIB_ELF64 ? "/64" : "");
		break;
	    case LIB_ELF_LIBC5:
	    case LIB_ELF_LIBC6:
		printf("(libc%d%s)", (libent[fd].flags & ~LIB_ELF64) + 3,
			libent[fd].flags & LIB_ELF64 ? "/64" : "");
		break;
	    default:
		printf("(unknown)");
		break;
	}
	printf(" => %s\n", strs + libent[fd].liboffset);
    }

    munmap (c,st.st_size);
}
#else
void cache_print(void)
{
    warnx("Cache support disabled\n");
}
#endif

void usage(void)
{
    fprintf(stderr,
	    "ldconfig - updates symlinks for shared libraries\n\n"
	    "Usage: ldconfig [-DvqnNX] [-f conf] [-C cache] [-r root] dir ...\n"
	    "       ldconfig -l [-Dv] lib ...\n"
	    "       ldconfig -p\n\nOptions:\n"
	    "\t-D:\t\tdebug mode, don't update links\n"
	    "\t-v:\t\tverbose mode, print things as we go\n"
	    "\t-q:\t\tquiet mode, don't print warnings\n"
	    "\t-n:\t\tdon't process standard directories\n"
	    "\t-N:\t\tdon't update the library cache\n"
	    "\t-X:\t\tdon't update the library links\n"
	    "\t-l:\t\tlibrary mode, manually link libraries\n"
	    "\t-p:\t\tprint the current library cache\n"
	    "\t-f conf :\tuse conf instead of %s\n"
	    "\t-C cache:\tuse cache instead of %s\n"
	    "\t-r root :\tfirst, do a chroot to the indicated directory\n"
	    "\tdir ... :\tdirectories to process\n"
	    "\tlib ... :\tlibraries to link\n\n",
	    LDSO_CONF, LDSO_CACHE
	   );
    exit(EXIT_FATAL);
}

#define DIR_SEP      ":, \t\n"
int main(int argc, char **argv)
{
    int i, c;
    int nodefault = 0;
    int printcache = 0;
    char *cp, *dir, *so;
    char *extpath;
    int libtype, islink;
    char *chroot_dir = NULL;

    prog = argv[0];
    opterr = 0;

    while ((c = getopt(argc, argv, "DvqnNXlpf:C:r:")) != EOF)
	switch (c)
	{
	    case 'D':
		debug = 1;		/* debug mode */
		nocache = 1;
		nolinks = 1;
		verbose = 1;
		break;
	    case 'v':
		verbose = 1;	/* verbose mode */
		break;
	    case 'q':
		if (verbose <= 0)
		    verbose = -1;	/* quiet mode */
		break;
	    case 'n':
		nodefault = 1;	/* no default dirs */
		nocache = 1;
		break;
	    case 'N':
		nocache = 1;	/* don't build cache */
		break;
	    case 'X':
		nolinks = 1;	/* don't update links */
		break;
	    case 'l':
		libmode = 1;	/* library mode */
		break;
	    case 'p':
		printcache = 1;	/* print cache */
		break;
	    case 'f':
		conffile = optarg;	/* alternate conf file */
		break;
	    case 'C':
		cachefile = optarg;	/* alternate cache file */
		break;
	    case 'r':
		chroot_dir = optarg;
		break;
	    default:
		usage();
		break;

		/* THE REST OF THESE ARE UNDOCUMENTED AND MAY BE REMOVED
		   IN FUTURE VERSIONS. */
	}

    if (chroot_dir && *chroot_dir) {
	if (chroot(chroot_dir) < 0)
	    err(EXIT_FATAL,"couldn't chroot to %s (%s)", chroot_dir, strerror(errno));
	if (chdir("/") < 0)
	    err(EXIT_FATAL,"couldn't chdir to / (%s)", strerror(errno));
    }

    /* allow me to introduce myself, hi, my name is ... */
    if (verbose > 0)
	printf("%s: uClibc version\n", argv[0]);

    if (printcache)
    {
	/* print the cache -- don't you trust me? */
	cache_print();
	exit(EXIT_OK);
    }
    else if (libmode)
    {
	/* so you want to do things manually, eh? */

	/* ok, if you're so smart, which libraries do we link? */
	for (i = optind; i < argc; i++)
	{
	    /* split into directory and file parts */
	    if (!(cp = strrchr(argv[i], '/')))
	    {
		dir = ".";	/* no dir, only a filename */
		cp = argv[i];
	    }
	    else
	    {
		if (cp == argv[i])
		    dir = "/";	/* file in root directory */
		else
		    dir = argv[i];
		*cp++ = '\0';	/* neither of the above */
	    }

	    /* we'd better do a little bit of checking */
	    if ((so = is_shlib(dir, cp, &libtype, &islink, LIB_ANY)) == NULL)
		err(EXIT_FATAL,"%s%s%s is not a shared library", dir,
			(*dir && strcmp(dir, "/")) ? "/" : "", cp);

	    /* so far, so good, maybe he knows what he's doing */
	    link_shlib(dir, cp, so);
	}
    }
    else
    {
	/* the lazy bum want's us to do all the work for him */

	/* don't cache dirs on the command line */
	int nocache_save = nocache;
	nocache = 1;

	/* OK, which directories should we do? */
	for (i = optind; i < argc; i++)
	    scan_dir(argv[i]);

	/* restore the desired caching state */
	nocache = nocache_save;

	/* look ma, no defaults */
	if (!nodefault)
	{
	    /* I guess the defaults aren't good enough */
	    if ((extpath = get_extpath()))
	    {
		for (cp = strtok(extpath, DIR_SEP); cp;
			cp = strtok(NULL, DIR_SEP))
		    scan_dir(cp);
		free(extpath);
	    }

	    scan_dir(UCLIBC_RUNTIME_PREFIX "/usr/X11R6/lib");
	    scan_dir(UCLIBC_RUNTIME_PREFIX "/usr/lib");
	    scan_dir(UCLIBC_RUNTIME_PREFIX "/lib");
	}

#ifdef USE_CACHE
	if (!nocache)
	    cache_write();
#endif
    }

    exit(EXIT_OK);
}

