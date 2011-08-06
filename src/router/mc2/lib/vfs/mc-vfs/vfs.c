/* Virtual File System switch code
   Copyright (C) 1995, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
   2007 Free Software Foundation, Inc.

   Written by: 1995 Miguel de Icaza
   1995 Jakub Jelinek
   1998 Pavel Machek

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/**
 * \file
 * \brief Source: Virtual File System switch code
 * \author Miguel de Icaza
 * \author Jakub Jelinek
 * \author Pavel Machek
 * \date 1995, 1998
 * \warning funtions like extfs_lstat() have right to destroy any
 * strings you pass to them. This is acutally ok as you g_strdup what
 * you are passing to them, anyway; still, beware.
 *
 * Namespace: exports *many* functions with vfs_ prefix; exports
 * parse_ls_lga and friends which do not have that prefix.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>             /* For atol() */
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>              /* is_digit() */
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "lib/global.h"
#include "lib/strutil.h"
#include "lib/util.h"
#include "lib/widget.h"         /* message() */

#ifdef HAVE_CHARSET
#include "lib/charsets.h"
#endif
#include "src/setup.h"          /* cd_symlinks */

#include "vfs-impl.h"
#include "utilvfs.h"
#include "gc.h"

#ifdef ENABLE_VFS_NET
#include "netutil.h"
#endif
#ifdef ENABLE_VFS_FTP
#include "ftpfs.h"
#endif
#ifdef ENABLE_VFS_SMB
#include "smbfs.h"
#endif

#include "local.h"

/*** global variables ****************************************************************************/

/*** file scope macro definitions ****************************************************************/

#if defined(_AIX) && !defined(NAME_MAX)
#define NAME_MAX FILENAME_MAX
#endif

#define VFS_FIRST_HANDLE 100

#define ISSLASH(a) (!a || (a == '/'))

/*** file scope type declarations ****************************************************************/

struct vfs_openfile
{
    int handle;
    struct vfs_class *vclass;
    void *fsinfo;
};

struct vfs_dirinfo
{
    DIR *info;
    GIConv converter;
};

/*** file scope variables ************************************************************************/

/** They keep track of the current directory */
static struct vfs_class *current_vfs;
static char *current_dir;

static GPtrArray *vfs_openfiles;
static long vfs_free_handle_list = -1;

static struct vfs_class *localfs_class;
static GString *vfs_str_buffer;

static struct vfs_class *vfs_list;

static struct dirent *mc_readdir_result = NULL;

static const struct
{
    const char *name;
    size_t name_len;
    const char *substitute;
} url_table[] =
{
    /* *INDENT-OFF* */
#ifdef ENABLE_VFS_FTP
    { "ftp://", 6, "/#ftp:" },
#endif
#ifdef ENABLE_VFS_FISH
    { "sh://", 5, "/#sh:" },
    { "ssh://", 6, "/#sh:" },
#endif
#ifdef ENABLE_VFS_SMB
    { "smb://", 6, "/#smb:" },
#endif
    { "a:", 2, "/#a" }
    /* *INDENT-ON* */
};

/*** file scope functions ************************************************************************/
/* --------------------------------------------------------------------------------------------- */
/**
 * Create new VFS handle and put it to the list
 */

static int
vfs_new_handle (struct vfs_class *vclass, void *fsinfo)
{
    struct vfs_openfile *h;

    h = g_new (struct vfs_openfile, 1);
    h->fsinfo = fsinfo;
    h->vclass = vclass;

    /* Allocate the first free handle */
    h->handle = vfs_free_handle_list;
    if (h->handle == -1)
    {
        /* No free allocated handles, allocate one */
        h->handle = vfs_openfiles->len;
        g_ptr_array_add (vfs_openfiles, h);
    }
    else
    {
        vfs_free_handle_list = (long) g_ptr_array_index (vfs_openfiles, vfs_free_handle_list);
        g_ptr_array_index (vfs_openfiles, h->handle) = h;
    }

    h->handle += VFS_FIRST_HANDLE;
    return h->handle;
}

/* --------------------------------------------------------------------------------------------- */
/** Find VFS class by file handle */

static struct vfs_class *
vfs_op (int handle)
{
    struct vfs_openfile *h;

    if (handle < VFS_FIRST_HANDLE || (guint) (handle - VFS_FIRST_HANDLE) >= vfs_openfiles->len)
        return NULL;

    h = (struct vfs_openfile *) g_ptr_array_index (vfs_openfiles, handle - VFS_FIRST_HANDLE);
    if (!h)
        return NULL;

    g_assert (h->handle == handle);

    return h->vclass;
}

/* --------------------------------------------------------------------------------------------- */
/** Find private file data by file handle */

static void *
vfs_info (int handle)
{
    struct vfs_openfile *h;

    if (handle < VFS_FIRST_HANDLE || (guint) (handle - VFS_FIRST_HANDLE) >= vfs_openfiles->len)
        return NULL;

    h = (struct vfs_openfile *) g_ptr_array_index (vfs_openfiles, handle - VFS_FIRST_HANDLE);
    if (!h)
        return NULL;

    g_assert (h->handle == handle);

    return h->fsinfo;
}

/* --------------------------------------------------------------------------------------------- */
/** Free open file data for given file handle */

static void
vfs_free_handle (int handle)
{
    const int idx = handle - VFS_FIRST_HANDLE;

    if (handle >= VFS_FIRST_HANDLE && (guint) idx < vfs_openfiles->len)
    {
        struct vfs_openfile *h;

        h = (struct vfs_openfile *) g_ptr_array_index (vfs_openfiles, idx);
        g_free (h);
        g_ptr_array_index (vfs_openfiles, idx) = (void *) vfs_free_handle_list;
        vfs_free_handle_list = idx;
    }
}


/* --------------------------------------------------------------------------------------------- */

/** Return VFS class for the given prefix */
static struct vfs_class *
vfs_prefix_to_class (char *prefix)
{
    struct vfs_class *vfs;

    /* Avoid last class (localfs) that would accept any prefix */
    for (vfs = vfs_list; vfs->next != NULL; vfs = vfs->next)
    {
        if (vfs->which != NULL)
        {
            if ((*vfs->which) (vfs, prefix) == -1)
                continue;
            return vfs;
        }
        if (vfs->prefix != NULL && strncmp (prefix, vfs->prefix, strlen (vfs->prefix)) == 0)
            return vfs;
    }
    return NULL;
}

/* --------------------------------------------------------------------------------------------- */

static gboolean
path_magic (const char *path)
{
    struct stat buf;

    return (stat (path, &buf) != 0);
}

/* --------------------------------------------------------------------------------------------- */

static struct vfs_class *
_vfs_get_class (char *path)
{
    char *semi;
    char *slash;
    struct vfs_class *ret;

    g_return_val_if_fail (path, NULL);

    semi = strrchr (path, '#');
    if (semi == NULL || !path_magic (path))
        return NULL;

    slash = strchr (semi, PATH_SEP);
    *semi = '\0';
    if (slash != NULL)
        *slash = '\0';

    ret = vfs_prefix_to_class (semi + 1);

    if (slash != NULL)
        *slash = PATH_SEP;
    if (ret == NULL)
        ret = _vfs_get_class (path);

    *semi = '#';
    return ret;
}

/* --------------------------------------------------------------------------------------------- */
/* now used only by vfs_translate_path, but could be used in other vfs 
 * plugin to automatic detect encoding
 * path - path to translate
 * size - how many bytes from path translate
 * defcnv - convertor, that is used as default, when path does not contain any
 *          #enc: subtring
 * buffer - used to store result of translation
 */

static estr_t
_vfs_translate_path (const char *path, int size, GIConv defcnv, GString * buffer)
{
    const char *semi;
    const char *slash;
    estr_t state = ESTR_SUCCESS;

    if (size == 0)
        return ESTR_SUCCESS;

    size = (size > 0) ? size : (signed int) strlen (path);

    /* try found /#enc: */
    semi = g_strrstr_len (path, size, PATH_SEP_STR VFS_ENCODING_PREFIX);
    if (semi != NULL)
    {
        char encoding[16];
        GIConv coder = INVALID_CONV;
        int ms;

        /* first must be translated part before /#enc: */
        ms = semi - path;

        state = _vfs_translate_path (path, ms, defcnv, buffer);

        if (state != ESTR_SUCCESS)
            return state;

        /* now can be translated part after #enc: */
        semi += strlen (VFS_ENCODING_PREFIX) + 1;       /* skip "/#enc:" */
        slash = strchr (semi, PATH_SEP);
        /* ignore slashes after size; */
        if (slash - path >= size)
            slash = NULL;

        ms = (slash != NULL) ? slash - semi : (int) strlen (semi);
        ms = min ((unsigned int) ms, sizeof (encoding) - 1);
        /* limit encoding size (ms) to path size (size) */
        if (semi + ms > path + size)
            ms = path + size - semi;
        memcpy (encoding, semi, ms);
        encoding[ms] = '\0';

#if HAVE_CHARSET
        if (is_supported_encoding (encoding))
            coder = str_crt_conv_to (encoding);
#endif

        if (coder != INVALID_CONV)
        {
            if (slash != NULL)
                state = str_vfs_convert_to (coder, slash, path + size - slash, buffer);
            else if (buffer->len == 0)
                g_string_append_c (buffer, PATH_SEP);
            str_close_conv (coder);
            return state;
        }

        errno = EINVAL;
        state = ESTR_FAILURE;
    }
    else
    {
        /* path can be translated whole at once */
        state = str_vfs_convert_to (defcnv, path, size, buffer);
    }

    return state;
}

/* --------------------------------------------------------------------------------------------- */

static int
ferrno (struct vfs_class *vfs)
{
    return vfs->ferrno ? (*vfs->ferrno) (vfs) : E_UNKNOWN;
    /* Hope that error message is obscure enough ;-) */
}

/* --------------------------------------------------------------------------------------------- */
/**
 * Return current directory.  If it's local, reread the current directory
 * from the OS.  You must g_strdup() whatever this function returns.
 */

static const char *
_vfs_get_cwd (void)
{
    char *trans;

    trans = vfs_translate_path_n (current_dir);

    if (_vfs_get_class (trans) == NULL)
    {
        const char *encoding = vfs_get_encoding (current_dir);

        if (encoding == NULL)
        {
            char *tmp;

            tmp = g_get_current_dir ();
            if (tmp != NULL)
            {                   /* One of the directories in the path is not readable */
                estr_t state;
                char *sys_cwd;

                g_string_set_size (vfs_str_buffer, 0);
                state = str_vfs_convert_from (str_cnv_from_term, tmp, vfs_str_buffer);
                g_free (tmp);

                sys_cwd = (state == ESTR_SUCCESS) ? g_strdup (vfs_str_buffer->str) : NULL;
                if (sys_cwd != NULL)
                {
                    struct stat my_stat, my_stat2;
                    /* Check if it is O.K. to use the current_dir */
                    if (cd_symlinks
                        && mc_stat (sys_cwd, &my_stat) == 0
                        && mc_stat (current_dir, &my_stat2) == 0
                        && my_stat.st_ino == my_stat2.st_ino && my_stat.st_dev == my_stat2.st_dev)
                        g_free (sys_cwd);
                    else
                    {
                        g_free (current_dir);
                        current_dir = sys_cwd;
                    }
                }
            }
        }
    }

    g_free (trans);
    return current_dir;
}

/* --------------------------------------------------------------------------------------------- */

static void
vfs_setup_wd (void)
{
    current_dir = g_strdup (PATH_SEP_STR);
    _vfs_get_cwd ();

    if (strlen (current_dir) > MC_MAXPATHLEN - 2)
        vfs_die ("Current dir too long.\n");

    current_vfs = vfs_get_class (current_dir);
}

/* --------------------------------------------------------------------------------------------- */

static char *
mc_def_getlocalcopy (const char *filename)
{
    char *tmp;
    int fdin, fdout;
    ssize_t i;
    char buffer[8192];
    struct stat mystat;

    fdin = mc_open (filename, O_RDONLY | O_LINEAR);
    if (fdin == -1)
        return NULL;

    fdout = vfs_mkstemps (&tmp, "vfs", filename);

    if (fdout == -1)
        goto fail;
    while ((i = mc_read (fdin, buffer, sizeof (buffer))) > 0)
    {
        if (write (fdout, buffer, i) != i)
            goto fail;
    }
    if (i == -1)
        goto fail;
    i = mc_close (fdin);
    fdin = -1;
    if (i == -1)
        goto fail;
    if (close (fdout) == -1)
    {
        fdout = -1;
        goto fail;
    }

    if (mc_stat (filename, &mystat) != -1)
        chmod (tmp, mystat.st_mode);

    return tmp;

  fail:
    if (fdout != -1)
        close (fdout);
    if (fdin != -1)
        mc_close (fdin);
    g_free (tmp);
    return NULL;
}

/* --------------------------------------------------------------------------------------------- */

static int
mc_def_ungetlocalcopy (struct vfs_class *vfs, const char *filename,
                       const char *local, int has_changed)
{
    int fdin = -1, fdout = -1;
    if (has_changed)
    {
        char buffer[8192];
        ssize_t i;

        if (!vfs->write)
            goto failed;

        fdin = open (local, O_RDONLY);
        if (fdin == -1)
            goto failed;
        fdout = mc_open (filename, O_WRONLY | O_TRUNC);
        if (fdout == -1)
            goto failed;
        while ((i = read (fdin, buffer, sizeof (buffer))) > 0)
            if (mc_write (fdout, buffer, (size_t) i) != i)
                goto failed;
        if (i == -1)
            goto failed;

        if (close (fdin) == -1)
        {
            fdin = -1;
            goto failed;
        }
        fdin = -1;
        if (mc_close (fdout) == -1)
        {
            fdout = -1;
            goto failed;
        }
    }
    unlink (local);
    return 0;

  failed:
    message (D_ERROR, _("Changes to file lost"), "%s", filename);
    if (fdout != -1)
        mc_close (fdout);
    if (fdin != -1)
        close (fdin);
    unlink (local);
    return -1;
}

/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

int
vfs_register_class (struct vfs_class *vfs)
{
    if (vfs->init != NULL)      /* vfs has own initialization function */
        if (!(*vfs->init) (vfs))        /* but it failed */
            return 0;

    vfs->next = vfs_list;
    vfs_list = vfs;

    return 1;
}

/* --------------------------------------------------------------------------------------------- */
/** Strip known vfs suffixes from a filename (possible improvement: strip
 *  suffix from last path component).
 *  \return a malloced string which has to be freed.
 */

char *
vfs_strip_suffix_from_filename (const char *filename)
{
    struct vfs_class *vfs;
    char *semi;
    char *p;

    if (filename == NULL)
        vfs_die ("vfs_strip_suffix_from_path got NULL: impossible");

    p = g_strdup (filename);
    semi = strrchr (p, '#');
    if (semi == NULL)
        return p;

    /* Avoid last class (localfs) that would accept any prefix */
    for (vfs = vfs_list; vfs->next != NULL; vfs = vfs->next)
    {
        if (vfs->which != NULL)
        {
            if ((*vfs->which) (vfs, semi + 1) == -1)
                continue;
            *semi = '\0';       /* Found valid suffix */
            return p;
        }
        if (vfs->prefix != NULL && strncmp (semi + 1, vfs->prefix, strlen (vfs->prefix)) == 0)
        {
            *semi = '\0';       /* Found valid suffix */
            return p;
        }
    }
    return p;
}

/* --------------------------------------------------------------------------------------------- */

/**
 * Splits path extracting vfs part.
 *
 * Splits path
 * \verbatim /p1#op/inpath \endverbatim
 * into
 * \verbatim inpath,op; \endverbatim
 * returns which vfs it is.
 * What is left in path is p1. You still want to g_free(path), you DON'T
 * want to free neither *inpath nor *op
 */

struct vfs_class *
vfs_split (char *path, char **inpath, char **op)
{
    char *semi;
    char *slash;
    struct vfs_class *ret;

    if (path == NULL)
        vfs_die ("Cannot split NULL");

    semi = strrchr (path, '#');
    if (semi == NULL || !path_magic (path))
        return NULL;

    slash = strchr (semi, PATH_SEP);
    *semi = '\0';

    if (op != NULL)
        *op = NULL;

    if (inpath != NULL)
        *inpath = NULL;

    if (slash != NULL)
        *slash = '\0';

    ret = vfs_prefix_to_class (semi + 1);
    if (ret != NULL)
    {
        if (op != NULL)
            *op = semi + 1;
        if (inpath != NULL)
            *inpath = slash != NULL ? slash + 1 : NULL;
        return ret;
    }

    if (slash != NULL)
        *slash = PATH_SEP;

    ret = vfs_split (path, inpath, op);
    *semi = '#';
    return ret;
}

/* --------------------------------------------------------------------------------------------- */

struct vfs_class *
vfs_get_class (const char *pathname)
{
    struct vfs_class *vfs;
    char *path = g_strdup (pathname);

    vfs = _vfs_get_class (path);
    g_free (path);

    if (!vfs)
        vfs = localfs_class;

    return vfs;
}

/* --------------------------------------------------------------------------------------------- */

const char *
vfs_get_encoding (const char *path)
{
    static char result[16];
    char *work;
    char *semi;
    char *slash;

    work = g_strdup (path);
    /* try found /#enc: */
    semi = g_strrstr (work, PATH_SEP_STR VFS_ENCODING_PREFIX);

    if (semi != NULL)
    {
        semi += strlen (VFS_ENCODING_PREFIX) + 1;       /* skip "/#enc:" */
        slash = strchr (semi, PATH_SEP);
        if (slash != NULL)
            slash[0] = '\0';

        g_strlcpy (result, semi, sizeof (result));
        g_free (work);
        return result;
    }
    else
    {
        g_free (work);
        return NULL;
    }
}

/* --------------------------------------------------------------------------------------------- */

char *
vfs_translate_path (const char *path)
{
    estr_t state;

    g_string_set_size (vfs_str_buffer, 0);
    state = _vfs_translate_path (path, -1, str_cnv_from_term, vfs_str_buffer);
    /*
       strict version
       return (state == 0) ? vfs_str_buffer->data : NULL;
     */
    return (state != ESTR_FAILURE) ? vfs_str_buffer->str : NULL;
}

/* --------------------------------------------------------------------------------------------- */

char *
vfs_translate_path_n (const char *path)
{
    char *result;

    result = vfs_translate_path (path);
    return (result != NULL) ? g_strdup (result) : NULL;
}

/* --------------------------------------------------------------------------------------------- */

char *
vfs_canon_and_translate (const char *path)
{
    char *canon;
    char *result;
    if (path == NULL)
        canon = g_strdup ("");
    else
        canon = vfs_canon (path);
    result = vfs_translate_path_n (canon);
    g_free (canon);
    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_open (const char *filename, int flags, ...)
{
    int mode = 0;
    void *info;
    va_list ap;
    char *file;
    struct vfs_class *vfs;

    file = vfs_canon_and_translate (filename);
    if (file == NULL)
        return -1;

    vfs = vfs_get_class (file);

    /* Get the mode flag */
    if (flags & O_CREAT)
    {
        va_start (ap, flags);
        mode = va_arg (ap, int);
        va_end (ap);
    }

    if (vfs->open == NULL)
    {
        g_free (file);
        errno = -EOPNOTSUPP;
        return -1;
    }

    info = vfs->open (vfs, file, flags, mode);  /* open must be supported */
    g_free (file);
    if (info == NULL)
    {
        errno = ferrno (vfs);
        return -1;
    }

    return vfs_new_handle (vfs, info);
}

/* --------------------------------------------------------------------------------------------- */

/* *INDENT-OFF* */

#define MC_NAMEOP(name, inarg, callarg) \
int mc_##name inarg \
{ \
    struct vfs_class *vfs; \
    int result; \
    char *mpath; \
    mpath = vfs_canon_and_translate (path); \
    if (mpath == NULL)  \
        return -1; \
    vfs = vfs_get_class (mpath); \
    if (vfs == NULL) \
    { \
        g_free (mpath); \
        return -1; \
    } \
    result = vfs->name != NULL ? vfs->name callarg : -1; \
    g_free (mpath); \
    if (result == -1) \
        errno = vfs->name != NULL ? ferrno (vfs) : E_NOTSUPP; \
    return result; \
}

MC_NAMEOP (chmod, (const char *path, mode_t mode), (vfs, mpath, mode))
MC_NAMEOP (chown, (const char *path, uid_t owner, gid_t group), (vfs, mpath, owner, group))
MC_NAMEOP (utime, (const char *path, struct utimbuf * times), (vfs, mpath, times))
MC_NAMEOP (readlink, (const char *path, char *buf, size_t bufsiz), (vfs, mpath, buf, bufsiz))
MC_NAMEOP (unlink, (const char *path), (vfs, mpath))
MC_NAMEOP (mkdir, (const char *path, mode_t mode), (vfs, mpath, mode))
MC_NAMEOP (rmdir, (const char *path), (vfs, mpath))
MC_NAMEOP (mknod, (const char *path, mode_t mode, dev_t dev), (vfs, mpath, mode, dev))

/* *INDENT-ON* */

/* --------------------------------------------------------------------------------------------- */

int
mc_symlink (const char *name1, const char *path)
{
    struct vfs_class *vfs;
    int result;
    char *mpath;
    char *lpath;
    char *tmp;

    mpath = vfs_canon_and_translate (path);
    if (mpath == NULL)
        return -1;

    tmp = g_strdup (name1);
    lpath = vfs_translate_path_n (tmp);
    g_free (tmp);

    if (lpath != NULL)
    {
        vfs = vfs_get_class (mpath);
        result = vfs->symlink != NULL ? vfs->symlink (vfs, lpath, mpath) : -1;
        g_free (lpath);
        g_free (mpath);

        if (result == -1)
            errno = vfs->symlink != NULL ? ferrno (vfs) : E_NOTSUPP;
        return result;
    }

    g_free (mpath);

    return -1;
}

/* --------------------------------------------------------------------------------------------- */

/* *INDENT-OFF* */

#define MC_HANDLEOP(name, inarg, callarg) \
ssize_t mc_##name inarg \
{ \
    struct vfs_class *vfs; \
    int result; \
    if (handle == -1) \
        return -1; \
    vfs = vfs_op (handle); \
    if (vfs == NULL) \
        return -1; \
    result = vfs->name != NULL ? vfs->name callarg : -1; \
    if (result == -1) \
        errno = vfs->name != NULL ? ferrno (vfs) : E_NOTSUPP; \
    return result; \
}

MC_HANDLEOP (read, (int handle, void *buffer, size_t count), (vfs_info (handle), buffer, count))
MC_HANDLEOP (write, (int handle, const void *buf, size_t nbyte), (vfs_info (handle), buf, nbyte))

/* --------------------------------------------------------------------------------------------- */

#define MC_RENAMEOP(name) \
int mc_##name (const char *fname1, const char *fname2) \
{ \
    struct vfs_class *vfs; \
    int result; \
    char *name2, *name1; \
    name1 = vfs_canon_and_translate (fname1); \
    if (name1 == NULL) \
        return -1; \
    name2 = vfs_canon_and_translate (fname2); \
    if (name2 == NULL) { \
        g_free (name1); \
        return -1; \
    } \
    vfs = vfs_get_class (name1); \
    if (vfs != vfs_get_class (name2)) \
    { \
        errno = EXDEV; \
        g_free (name1); \
        g_free (name2); \
        return -1; \
    } \
    result = vfs->name != NULL ? vfs->name (vfs, name1, name2) : -1; \
    g_free (name1); \
    g_free (name2); \
    if (result == -1) \
        errno = vfs->name != NULL ? ferrno (vfs) : E_NOTSUPP; \
    return result; \
}

MC_RENAMEOP (link)
MC_RENAMEOP (rename)

/* *INDENT-ON* */

/* --------------------------------------------------------------------------------------------- */

int
mc_ctl (int handle, int ctlop, void *arg)
{
    struct vfs_class *vfs = vfs_op (handle);

    if (vfs == NULL)
        return 0;

    return vfs->ctl ? (*vfs->ctl) (vfs_info (handle), ctlop, arg) : 0;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_setctl (const char *path, int ctlop, void *arg)
{
    struct vfs_class *vfs;
    int result = -1;
    char *mpath;

    if (path == NULL)
        vfs_die ("You don't want to pass NULL to mc_setctl.");

    mpath = vfs_canon_and_translate (path);
    if (mpath != NULL)
    {
        vfs = vfs_get_class (mpath);
        result = vfs->setctl != NULL ? vfs->setctl (vfs, mpath, ctlop, arg) : 0;
        g_free (mpath);
    }

    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_close (int handle)
{
    struct vfs_class *vfs;
    int result;

    if (handle == -1 || !vfs_info (handle))
        return -1;

    vfs = vfs_op (handle);
    if (vfs == NULL)
        return -1;

    if (handle < 3)
        return close (handle);

    if (!vfs->close)
        vfs_die ("VFS must support close.\n");
    result = (*vfs->close) (vfs_info (handle));
    vfs_free_handle (handle);
    if (result == -1)
        errno = ferrno (vfs);

    return result;
}

/* --------------------------------------------------------------------------------------------- */

DIR *
mc_opendir (const char *dirname)
{
    int handle, *handlep;
    void *info;
    struct vfs_class *vfs;
    char *canon;
    char *dname;
    struct vfs_dirinfo *dirinfo;
    const char *encoding;

    canon = vfs_canon (dirname);
    dname = vfs_translate_path_n (canon);

    if (dname != NULL)
    {
        vfs = vfs_get_class (dname);
        info = vfs->opendir ? (*vfs->opendir) (vfs, dname) : NULL;
        g_free (dname);

        if (info == NULL)
        {
            errno = vfs->opendir ? ferrno (vfs) : E_NOTSUPP;
            g_free (canon);
            return NULL;
        }

        dirinfo = g_new (struct vfs_dirinfo, 1);
        dirinfo->info = info;

        encoding = vfs_get_encoding (canon);
        g_free (canon);
        dirinfo->converter = (encoding != NULL) ? str_crt_conv_from (encoding) : str_cnv_from_term;
        if (dirinfo->converter == INVALID_CONV)
            dirinfo->converter = str_cnv_from_term;

        handle = vfs_new_handle (vfs, dirinfo);

        handlep = g_new (int, 1);
        *handlep = handle;
        return (DIR *) handlep;
    }
    else
    {
        g_free (canon);
        return NULL;
    }
}

/* --------------------------------------------------------------------------------------------- */

struct dirent *
mc_readdir (DIR * dirp)
{
    int handle;
    struct vfs_class *vfs;
    struct dirent *entry = NULL;
    struct vfs_dirinfo *dirinfo;
    estr_t state;

    if (!mc_readdir_result)
    {
        /* We can't just allocate struct dirent as (see man dirent.h)
         * struct dirent has VERY nonnaive semantics of allocating
         * d_name in it. Moreover, linux's glibc-2.9 allocates dirents _less_,
         * than 'sizeof (struct dirent)' making full bitwise (sizeof dirent) copy
         * heap corrupter. So, allocate longliving dirent with at least
         * (MAXNAMLEN + 1) for d_name in it.
         * Strictly saying resulting dirent is unusable as we don't adjust internal
         * structures, holding dirent size. But we don't use it in libc infrastructure.
         * TODO: to make simpler homemade dirent-alike structure.
         */
        mc_readdir_result = (struct dirent *) g_malloc (sizeof (struct dirent) + MAXNAMLEN + 1);
    }

    if (!dirp)
    {
        errno = EFAULT;
        return NULL;
    }
    handle = *(int *) dirp;

    vfs = vfs_op (handle);
    if (vfs == NULL)
        return NULL;

    dirinfo = vfs_info (handle);
    if (vfs->readdir)
    {
        entry = (*vfs->readdir) (dirinfo->info);
        if (entry == NULL)
            return NULL;
        g_string_set_size (vfs_str_buffer, 0);
        state = str_vfs_convert_from (dirinfo->converter, entry->d_name, vfs_str_buffer);
        mc_readdir_result->d_ino = entry->d_ino;
        g_strlcpy (mc_readdir_result->d_name, vfs_str_buffer->str, MAXNAMLEN + 1);
    }
    if (entry == NULL)
        errno = vfs->readdir ? ferrno (vfs) : E_NOTSUPP;
    return (entry != NULL) ? mc_readdir_result : NULL;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_closedir (DIR * dirp)
{
    int handle = *(int *) dirp;
    struct vfs_class *vfs;
    int result = -1;

    vfs = vfs_op (handle);
    if (vfs != NULL)
    {
        struct vfs_dirinfo *dirinfo;

        dirinfo = vfs_info (handle);
        if (dirinfo->converter != str_cnv_from_term)
            str_close_conv (dirinfo->converter);

        result = vfs->closedir ? (*vfs->closedir) (dirinfo->info) : -1;
        vfs_free_handle (handle);
        g_free (dirinfo);
    }
    g_free (dirp);
    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_stat (const char *filename, struct stat *buf)
{
    struct vfs_class *vfs;
    int result;
    char *path;

    path = vfs_canon_and_translate (filename);

    if (path == NULL)
        return -1;

    vfs = vfs_get_class (path);

    if (vfs == NULL)
    {
        g_free (path);
        return -1;
    }

    result = vfs->stat ? (*vfs->stat) (vfs, path, buf) : -1;

    g_free (path);

    if (result == -1)
        errno = vfs->name ? ferrno (vfs) : E_NOTSUPP;
    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_lstat (const char *filename, struct stat *buf)
{
    struct vfs_class *vfs;
    int result;
    char *path;

    path = vfs_canon_and_translate (filename);

    if (path == NULL)
        return -1;

    vfs = vfs_get_class (path);
    if (vfs == NULL)
    {
        g_free (path);
        return -1;
    }

    result = vfs->lstat ? (*vfs->lstat) (vfs, path, buf) : -1;
    g_free (path);
    if (result == -1)
        errno = vfs->name ? ferrno (vfs) : E_NOTSUPP;
    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_fstat (int handle, struct stat *buf)
{
    struct vfs_class *vfs;
    int result;

    if (handle == -1)
        return -1;

    vfs = vfs_op (handle);
    if (vfs == NULL)
        return -1;

    result = vfs->fstat ? (*vfs->fstat) (vfs_info (handle), buf) : -1;
    if (result == -1)
        errno = vfs->name ? ferrno (vfs) : E_NOTSUPP;
    return result;
}

/* --------------------------------------------------------------------------------------------- */
/**
 * Return current directory. If it's local, reread the current directory
 * from the OS. Put directory to the provided buffer.
 */

char *
mc_get_current_wd (char *buffer, size_t size)
{
    const char *cwd = _vfs_get_cwd ();

    g_strlcpy (buffer, cwd, size);
    return buffer;
}

/* --------------------------------------------------------------------------------------------- */
/**
 * Return current directory without any OS calls.
 */

char *
vfs_get_current_dir (void)
{
    return current_dir;
}

/* --------------------------------------------------------------------------------------------- */

off_t
mc_lseek (int fd, off_t offset, int whence)
{
    struct vfs_class *vfs;
    off_t result;

    if (fd == -1)
        return -1;

    vfs = vfs_op (fd);
    if (vfs == NULL)
        return -1;

    result = vfs->lseek ? (*vfs->lseek) (vfs_info (fd), offset, whence) : -1;
    if (result == -1)
        errno = vfs->lseek ? ferrno (vfs) : E_NOTSUPP;
    return result;
}

/* --------------------------------------------------------------------------------------------- */
/**
 * remove //, /./ and /../
 */

char *
vfs_canon (const char *path)
{
    if (!path)
        vfs_die ("Cannot canonicalize NULL");

    /* Relative to current directory */
    if (*path != PATH_SEP)
    {
        char *local, *result;

        local = concat_dir_and_file (current_dir, path);

        result = vfs_canon (local);
        g_free (local);
        return result;
    }

    /*
     * So we have path of following form:
     * /p1/p2#op/.././././p3#op/p4. Good luck.
     */
    {
        char *result = g_strdup (path);
        canonicalize_pathname (result);
        return result;
    }
}

/* --------------------------------------------------------------------------------------------- */
/**
 * VFS chdir.
 * Return 0 on success, -1 on failure.
 */

int
mc_chdir (const char *path)
{
    char *new_dir;
    char *trans_dir;
    struct vfs_class *old_vfs, *new_vfs;
    vfsid old_vfsid;
    int result;

    new_dir = vfs_canon (path);
    trans_dir = vfs_translate_path_n (new_dir);
    if (trans_dir != NULL)
    {
        new_vfs = vfs_get_class (trans_dir);
        if (!new_vfs->chdir)
        {
            g_free (new_dir);
            g_free (trans_dir);
            return -1;
        }

        result = (*new_vfs->chdir) (new_vfs, trans_dir);

        if (result == -1)
        {
            errno = ferrno (new_vfs);
            g_free (new_dir);
            g_free (trans_dir);
            return -1;
        }

        old_vfsid = vfs_getid (current_vfs, current_dir);
        old_vfs = current_vfs;

        /* Actually change directory */
        g_free (current_dir);
        current_dir = new_dir;
        current_vfs = new_vfs;

        /* This function uses the new current_dir implicitly */
        vfs_stamp_create (old_vfs, old_vfsid);

        /* Sometimes we assume no trailing slash on cwd */
        if (*current_dir)
        {
            char *p;
            p = strchr (current_dir, 0) - 1;
            if (*p == PATH_SEP && p > current_dir)
                *p = 0;
        }

        g_free (trans_dir);
        return 0;
    }
    else
    {
        g_free (new_dir);
        return -1;
    }
}

/* --------------------------------------------------------------------------------------------- */
/* Return TRUE is the current VFS class is local */

gboolean
vfs_current_is_local (void)
{
    return (current_vfs->flags & VFSF_LOCAL) != 0;
}

/* --------------------------------------------------------------------------------------------- */
/* Return flags of the VFS class of the given filename */

vfs_class_flags_t
vfs_file_class_flags (const char *filename)
{
    struct vfs_class *vfs;
    char *fname;

    fname = vfs_canon_and_translate (filename);
    if (fname == NULL)
        return VFSF_UNKNOWN;

    vfs = vfs_get_class (fname);
    g_free (fname);
    return vfs->flags;
}

/* --------------------------------------------------------------------------------------------- */

char *
mc_getlocalcopy (const char *pathname)
{
    char *result = NULL;
    char *path;

    path = vfs_canon_and_translate (pathname);
    if (path != NULL)
    {
        struct vfs_class *vfs = vfs_get_class (path);

        result = vfs->getlocalcopy != NULL ?
            vfs->getlocalcopy (vfs, path) : mc_def_getlocalcopy (path);
        g_free (path);
        if (result == NULL)
            errno = ferrno (vfs);
    }

    return result;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_ungetlocalcopy (const char *pathname, const char *local, int has_changed)
{
    int return_value = -1;
    char *path;

    path = vfs_canon_and_translate (pathname);
    if (path != NULL)
    {
        struct vfs_class *vfs = vfs_get_class (path);

        return_value = vfs->ungetlocalcopy != NULL ?
            vfs->ungetlocalcopy (vfs, path, local, has_changed) :
            mc_def_ungetlocalcopy (vfs, path, local, has_changed);
        g_free (path);
    }

    return return_value;
}

/* --------------------------------------------------------------------------------------------- */

void
vfs_init (void)
{
    /* create the VFS handle array */
    vfs_openfiles = g_ptr_array_new ();

    vfs_str_buffer = g_string_new ("");
    /* localfs needs to be the first one */
    init_localfs ();
    /* fallback value for vfs_get_class() */
    localfs_class = vfs_list;

#ifdef ENABLE_VFS_CPIO
    init_cpiofs ();
#endif /* ENABLE_VFS_CPIO */
#ifdef ENABLE_VFS_TAR
    init_tarfs ();
#endif /* ENABLE_VFS_TAR */
#ifdef ENABLE_VFS_SFS
    init_sfs ();
#endif /* ENABLE_VFS_SFS */
#ifdef ENABLE_VFS_EXTFS
    init_extfs ();
#endif /* ENABLE_VFS_EXTFS */
#ifdef ENABLE_VFS_UNDELFS
    init_undelfs ();
#endif /* ENABLE_VFS_UNDELFS */

#ifdef ENABLE_VFS_FTP
    init_ftpfs ();
#endif /* ENABLE_VFS_FTP */
#ifdef ENABLE_VFS_FISH
    init_fish ();
#endif /* ENABLE_VFS_FISH */
#ifdef ENABLE_VFS_SMB
    init_smbfs ();
#endif /* ENABLE_VFS_SMB */

    vfs_setup_wd ();
}

/* --------------------------------------------------------------------------------------------- */

void
vfs_shut (void)
{
    struct vfs_class *vfs;

    vfs_gc_done ();

    g_free (current_dir);

    for (vfs = vfs_list; vfs; vfs = vfs->next)
        if (vfs->done)
            (*vfs->done) (vfs);

    g_ptr_array_free (vfs_openfiles, TRUE);
    g_string_free (vfs_str_buffer, TRUE);
    g_free (mc_readdir_result);
}

/* --------------------------------------------------------------------------------------------- */
/**
 * These ones grab information from the VFS
 *  and handles them to an upper layer
 */

void
vfs_fill_names (fill_names_f func)
{
    struct vfs_class *vfs;

    for (vfs = vfs_list; vfs; vfs = vfs->next)
        if (vfs->fill_names)
            (*vfs->fill_names) (vfs, func);
}

/* --------------------------------------------------------------------------------------------- */
/**
 * Returns vfs path corresponding to given url. If passed string is
 * not recognized as url, g_strdup(url) is returned.
 */

char *
vfs_translate_url (const char *url)
{
    size_t i;

    for (i = 0; i < sizeof (url_table) / sizeof (url_table[0]); i++)
        if (strncmp (url, url_table[i].name, url_table[i].name_len) == 0)
            return g_strconcat (url_table[i].substitute, url + url_table[i].name_len,
                                (char *) NULL);

    return g_strdup (url);
}

/* --------------------------------------------------------------------------------------------- */

gboolean
vfs_file_is_local (const char *filename)
{
    return (vfs_file_class_flags (filename) & VFSF_LOCAL) != 0;
}

/* --------------------------------------------------------------------------------------------- */
