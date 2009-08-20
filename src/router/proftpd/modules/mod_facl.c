/*
 * ProFTPD - FTP server daemon
 * Copyright (c) 2004-2007 The ProFTPD Project team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 *
 * As a special exemption, the ProFTPD Project team and other respective
 * copyright holders give permission to link this program with OpenSSL, and
 * distribute the resulting executable, without including the source code
 * for OpenSSL in the source distribution.
 */

/*
 * POSIX ACL checking code (aka POSIX.1e hell)
 * $Id: mod_facl.c,v 1.12 2008/12/17 17:46:47 castaglia Exp $
 */

#include "conf.h"

#define MOD_FACL_VERSION		"mod_facl/0.4"

/* Make sure the version of proftpd is as necessary. */
#if PROFTPD_VERSION_NUMBER < 0x0001030101
# error "ProFTPD 1.3.1rc1 or later required"
#endif

module facl_module;

static int facl_engine = TRUE;
static const char *trace_channel = "facl";

#ifdef HAVE_POSIX_ACL

#ifdef HAVE_SYS_ACL_H
# include <sys/acl.h>
#endif

/* This header appears on Linux. */
#ifdef HAVE_ACL_LIBACL_H
# include <acl/libacl.h>
#endif

static int is_errno_eperm(void) {
  if (errno == EPERM)
    return 1;

#ifdef EOPNOTSUPP
  if (errno == EOPNOTSUPP)
    return 1;
#endif /* !EOPNOTSUPP */

  return 0;
}

/* Copied directory from src/fsio.c, since these functions are not
 * accessible outside of that file.
 */
static int sys_access(pr_fs_t *fs, const char *path, int mode, uid_t uid,
    gid_t gid, array_header *suppl_gids) {
  mode_t mask;
  struct stat st;

  pr_fs_clear_cache();
  if (pr_fsio_stat(path, &st) < 0)
    return -1;

  /* Root always succeeds. */
  if (uid == PR_ROOT_UID)
    return 0;

  /* Initialize mask to reflect the permission bits that are applicable for
   * the given user. mask contains the user-bits if the user ID equals the
   * ID of the file owner. mask contains the group bits if the group ID
   * belongs to the group of the file. mask will always contain the other
   * bits of the permission bits.
   */
  mask = S_IROTH|S_IWOTH|S_IXOTH;

  if (st.st_uid == uid)
    mask |= S_IRUSR|S_IWUSR|S_IXUSR;

  /* Check the current group, as well as all supplementary groups.
   * Fortunately, we have this information cached, so accessing it is
   * almost free.
   */
  if (st.st_gid == gid) {
    mask |= S_IRGRP|S_IWGRP|S_IXGRP;

  } else {
    if (suppl_gids) {
      register unsigned int i = 0;

      for (i = 0; i < suppl_gids->nelts; i++) {
        if (st.st_gid == ((gid_t *) suppl_gids->elts)[i]) {
          mask |= S_IRGRP|S_IWGRP|S_IXGRP;
          break;
        }
      }
    }
  }

  mask &= st.st_mode;

  /* Perform requested access checks. */
  if (mode & R_OK) {
    if (!(mask & (S_IRUSR|S_IRGRP|S_IROTH))) {
      errno = EACCES;
      return -1;
    }
  }

  if (mode & W_OK) {
    if (!(mask & (S_IWUSR|S_IWGRP|S_IWOTH))) {
      errno = EACCES;
      return -1;
    }
  }

  if (mode & X_OK) {
    if (!(mask & (S_IXUSR|S_IXGRP|S_IXOTH))) {
      errno = EACCES;
      return -1;
    }
  }

  /* F_OK already checked by checking the return value of stat. */
  return 0;
}

static int sys_faccess(pr_fh_t *fh, int mode, uid_t uid, gid_t gid,
    array_header *suppl_gids) {
  return sys_access(fh->fh_fs, fh->fh_path, mode, uid, gid, suppl_gids);
}

#if defined(HAVE_BSD_POSIX_ACL) || defined(HAVE_LINUX_POSIX_ACL)
static acl_perm_t get_facl_perm_for_mode(int mode) {
  acl_perm_t res;

  memset(&res, 0, sizeof(acl_perm_t));

  if (mode & R_OK)
    res |= ACL_READ;

  if (mode & W_OK)
    res |= ACL_WRITE;

  if (mode & X_OK)
    res |= ACL_EXECUTE;

  return res;
}
#endif

static int check_facl(pool *p, const char *path, int mode, void *acl, int nents,
    struct stat *st, uid_t uid, gid_t gid, array_header *suppl_gids) {
# if defined(HAVE_BSD_POSIX_ACL) || defined(HAVE_LINUX_POSIX_ACL)
  register unsigned int i;
  int have_access_entry = FALSE, res = -1;
  pool *acl_pool;
  acl_t facl = acl;
  acl_entry_t ae;
  acl_tag_t ae_type;
  acl_entry_t acl_user_entry = NULL;
  acl_entry_t acl_group_entry = NULL;
  acl_entry_t acl_other_entry = NULL;
  acl_entry_t acl_mask_entry = NULL;
  array_header *acl_groups;
  array_header *acl_users;

  /* Iterate through all of the ACL entries, sorting them for later
   * checking.
   */
  res = acl_get_entry(facl, ACL_FIRST_ENTRY, &ae);
  if (res < 0) {
    pr_trace_msg(trace_channel, 8,
      "unable to retrieve first ACL entry for '%s': [%d] %s", path,
      errno, strerror(errno));
    errno = EACCES;
    return -1;
  }

  if (res == 0) {
    pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s' has no entries",
      path);
    errno = EACCES;
    return -1;
  }

  acl_pool = make_sub_pool(p);
  acl_groups = make_array(acl_pool, 1, sizeof(acl_entry_t));
  acl_users = make_array(acl_pool, 1, sizeof(acl_entry_t));

  while (res > 0) {
    if (acl_get_tag_type(ae, &ae_type) < 0) {
      pr_trace_msg(trace_channel, 5,
        "error retrieving type of ACL entry for '%s': %s", path,
        strerror(errno));
      res = acl_get_entry(facl, ACL_NEXT_ENTRY, &ae);
      continue;
    }

    if (ae_type & ACL_USER_OBJ) {
      acl_user_entry = ae;

    } else if (ae_type & ACL_USER) {
      acl_entry_t *ae_dup = push_array(acl_users);
      *ae_dup = ae;

    } else if (ae_type & ACL_GROUP_OBJ) {
      acl_group_entry = ae;

    } else if (ae_type & ACL_GROUP) {
      acl_entry_t *ae_dup = push_array(acl_groups);
      *ae_dup = ae;

    } else if (ae_type & ACL_OTHER) {
      acl_other_entry = ae;

    } else if (ae_type & ACL_MASK) {
      acl_mask_entry = ae;
    }

    res = acl_get_entry(facl, ACL_NEXT_ENTRY, &ae);
  }
  ae = NULL;

  /* Select the ACL entry that determines access. */
  res = -1;

  /* 1. If the given user ID matches the file owner, use that entry for
   *    access.
   */
  if (uid == st->st_uid) {
    /* Check the acl_user_entry for access. */
    ae = acl_user_entry;
    ae_type = ACL_USER_OBJ;
    have_access_entry = TRUE;

    pr_trace_msg(trace_channel, 10, "user ID %lu matches ACL owner user ID",
      (unsigned long) uid);
  }

  /* 2. If not matched above, and if the given user ID matches one of the
   *    named user entries, use that entry for access.
   */
  for (i = 0; !have_access_entry && i < acl_users->nelts; i++) {
    acl_entry_t e = ((acl_entry_t *) acl_users->elts)[i];

    if (uid == *((uid_t *) acl_get_qualifier(e))) {

      /* Check this entry for access. Note that it'll need to
       * be modified by the mask, if any, later.
       */
      ae = e;
      ae_type = ACL_USER;
      have_access_entry = TRUE;

      pr_trace_msg(trace_channel, 10,
        "user ID %lu matches ACL allowed users list", (unsigned long) uid);

      break;
    }
  }

  /* 3. If not matched above, and if one of the group IDs matches the
   *    group owner entry, and the group owner entry contains the
   *    requested permissions, use that entry for access.
   */
  if (!have_access_entry &&
      gid == st->st_gid) {
    int ret;

    /* Check the acl_group_entry for access. First though, we need to
     * see if the acl_group_entry contains the requested permissions.
     */
    acl_permset_t perms;
    if (acl_get_permset(acl_group_entry, &perms) < 0) {
      pr_trace_msg(trace_channel, 5, "error retrieving permission set: %s",
        strerror(errno));
    }

#  if defined(HAVE_BSD_POSIX_ACL)
    ret = acl_get_perm_np(perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
    ret = acl_get_perm(perms, get_facl_perm_for_mode(mode));
#  endif

    if (ret == 1) {
      ae = acl_group_entry;
      ae_type = ACL_GROUP_OBJ;
      have_access_entry = TRUE;

      pr_trace_msg(trace_channel, 10,
        "primary group ID %lu matches ACL owner group ID",
        (unsigned long) gid);

    } else if (ret < 0) {
      pr_trace_msg(trace_channel, 5,
        "error checking permissions in permission set: %s", strerror(errno));
    }
  }

  if (suppl_gids) {
    for (i = 0; !have_access_entry && i < suppl_gids->nelts; i++) {
      gid_t suppl_gid = ((gid_t *) suppl_gids->elts)[i];

      if (suppl_gid == st->st_gid) {
        int ret;

        /* Check the acl_group_entry for access. First though, we need to
         * see if the acl_group_entry contains the requested permissions.
         */
        acl_permset_t perms;
        if (acl_get_permset(acl_group_entry, &perms) < 0) {
          pr_trace_msg(trace_channel, 5, "error retrieving permission set: %s",
            strerror(errno));
        }

#  if defined(HAVE_BSD_POSIX_ACL)
        ret = acl_get_perm_np(perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
        ret = acl_get_perm(perms, get_facl_perm_for_mode(mode));
#  endif

        if (ret == 1) {
          ae = acl_group_entry;
          ae_type = ACL_GROUP_OBJ;
          have_access_entry = TRUE;

          pr_trace_msg(trace_channel, 10,
            "supplemental group ID %lu matches ACL owner group ID",
            (unsigned long) suppl_gid);

          break;

        } else if (ret < 0) {
          pr_trace_msg(trace_channel, 5,
            "error checking permissions in permission set: %s",
            strerror(errno));
        }
      }
    }
  }

  /* 4. If not matched above, and if one of the group IDs matches one
   *    of the named group entries, and that entry contains the requested
   *    permissions, use that entry for access.
   */
  for (i = 0; !have_access_entry && i < acl_groups->nelts; i++) {
    acl_entry_t e = ((acl_entry_t *) acl_groups->elts)[i];

    if (gid == *((gid_t *) acl_get_qualifier(e))) {
      int ret;

      /* Check this entry for access. Note that it'll need to
       * be modified by the mask, if any, later.
       */
      acl_permset_t perms;
      if (acl_get_permset(e, &perms) < 0) {
        pr_trace_msg(trace_channel, 5, "error retrieving permission set: %s",
          strerror(errno));
      }

#  if defined(HAVE_BSD_POSIX_ACL)
      ret = acl_get_perm_np(perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
      ret = acl_get_perm(perms, get_facl_perm_for_mode(mode));
#  endif

      if (ret == 1) {
        ae = e;
        ae_type = ACL_GROUP;
        have_access_entry = TRUE;

        pr_trace_msg(trace_channel, 10,
          "primary group ID %lu matches ACL allowed groups list",
          (unsigned long) gid);

        break;

      } else if (ret < 0) {
        pr_trace_msg(trace_channel, 5,
          "error checking permissions in permission set: %s", strerror(errno));
      }
    }

    if (suppl_gids) {
      register unsigned int j;

      for (j = 0; !have_access_entry && j < suppl_gids->nelts; j++) {
        gid_t suppl_gid = ((gid_t *) suppl_gids->elts)[j];

        if (suppl_gid == *((gid_t *) acl_get_qualifier(e))) {
          int ret;

          /* Check this entry for access. Note that it'll need to
           * be modified by the mask, if any, later.
           */
          acl_permset_t perms;
          if (acl_get_permset(e, &perms) < 0) {
            pr_trace_msg(trace_channel, 5,
              "error retrieving permission set: %s", strerror(errno));
          }

#  if defined(HAVE_BSD_POSIX_ACL)
          ret = acl_get_perm_np(perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
          ret = acl_get_perm(perms, get_facl_perm_for_mode(mode));
#  endif

          if (ret == 1) {
            ae = e;
            ae_type = ACL_GROUP;
            have_access_entry = TRUE;

            pr_trace_msg(trace_channel, 10,
              "supplemental group ID %lu matches ACL allowed groups list",
              (unsigned long) suppl_gid);

            break;

          } else if (ret < 0) {
            pr_trace_msg(trace_channel, 5,
              "error checking permissions in permission set: %s",
              strerror(errno));
          }
        }
      }
    }
  }

  /* 5. If not matched above, and if one of the group IDs matches
   *    the group owner or any of the named group entries, but neither
   *    the group owner entry nor any of the named group entries contains
   *    the requested permissions, access is denied.
   */

  /* XXX implement this condition properly */

  /* 6. If not matched above, the other entry determines access.
   */
  if (!have_access_entry) {
    ae = acl_other_entry;
    ae_type = ACL_OTHER;
    have_access_entry = TRUE;

    pr_trace_msg(trace_channel, 10, "using ACL 'other' list");
  }

  /* Access determination:
   *
   *  If either the user owner entry or other entry were used, and the
   *  entry contains the requested permissions, access is permitted.
   *
   *  Otherwise, if the selected entry and the mask entry both contain
   *  the requested permissions (or there is no mask entry), access is
   *  permitted.
   *
   *  Otherwise, access is denied.
   */
  switch (ae_type) {
    case ACL_USER_OBJ:
    case ACL_OTHER: {
      int ret;

      acl_permset_t perms;
      if (acl_get_permset(ae, &perms) < 0) {
        pr_trace_msg(trace_channel, 5, "error retrieving permission set: %s",
          strerror(errno));
      }

#  if defined(HAVE_BSD_POSIX_ACL)
      ret = acl_get_perm_np(perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
      ret = acl_get_perm(perms, get_facl_perm_for_mode(mode));
#  endif

     if (ret == 1) {
        res = 0;

      } else if (ret < 0) {
        pr_trace_msg(trace_channel, 5,
          "error checking permissions in permission set: %s", strerror(errno));
      }

      break;
    }

    default: {
      int ret1, ret2;
      acl_permset_t ent_perms, mask_perms;

      if (acl_get_permset(ae, &ent_perms) < 0) {
        pr_trace_msg(trace_channel, 5, "error retrieving permission set: %s",
          strerror(errno));
      }

#  if defined(HAVE_BSD_POSIX_ACL)
      ret1 = acl_get_perm_np(ent_perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
      ret1 = acl_get_perm(ent_perms, get_facl_perm_for_mode(mode));
#  endif

      if (acl_mask_entry != NULL) {
        if (acl_get_permset(acl_mask_entry, &mask_perms) < 0) {
          pr_trace_msg(trace_channel, 5,
            "error retrieving mask permission set: %s", strerror(errno));
        }

#  if defined(HAVE_BSD_POSIX_ACL)
        ret2 = acl_get_perm_np(mask_perms, get_facl_perm_for_mode(mode));
#  elif defined(HAVE_LINUX_POSIX_ACL)
        ret2 = acl_get_perm(mask_perms, get_facl_perm_for_mode(mode));
#  endif

      } else {
        /* If there is no mask entry, then access should be granted. */
        ret2 = 1;
      }

      if (ret1 == 1 && ret2 == 1) {
        res = 0;

      } else {
        if (ret1 < 0) {
          pr_trace_msg(trace_channel, 5,
            "error checking permissions in entry permission set: %s",
            strerror(errno));
        }

        if (ret2 < 0) {
          pr_trace_msg(trace_channel, 5,
            "error checking permissions in mask permission set: %s",
            strerror(errno));
        }
      }

      break;
    }
  }

  destroy_pool(acl_pool);

  if (res < 0) {
    errno = EACCES;
    pr_trace_msg(trace_channel, 3,
      "returning EACCES for path '%s', user ID %lu", path,
      (unsigned long) uid);
  }

  return res;

# elif defined(HAVE_SOLARIS_POSIX_ACL)
  register unsigned int i;
  int have_access_entry = FALSE, have_mask_entry = FALSE, idx, res = -1;
  pool *acl_pool;
  aclent_t *acls = acl;
  aclent_t ae;
  int ae_type = 0;
  aclent_t acl_user_entry;
  aclent_t acl_group_entry;
  aclent_t acl_other_entry;
  aclent_t acl_mask_entry;
  array_header *acl_groups;
  array_header *acl_users;

  /* In the absence of any clear documentation, I'll assume that
   * Solaris ACLs follow the same selection and checking algorithm
   * as do BSD and Linux.
   */

  res = aclcheck(acls, nents, &idx);
  switch (res) {
    case 0:
      break;

    case GRP_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "too many GROUP entries");
      errno = EACCES;
      return -1;

    case USER_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "too many USER entries");
      errno = EACCES;
      return -1;

    case OTHER_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "too many OTHER entries");
      errno = EACCES;
      return -1;

    case CLASS_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "too many CLASS entries");
      errno = EACCES;
      return -1;

    case DUPLICATE_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "duplicate entries");
      errno = EACCES;
      return -1;

    case MISS_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "missing required entry");
      errno = EACCES;
      return -1;

    case MEM_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "Out of memory!");
      errno = EACCES;
      return -1;

    case ENTRY_ERROR:
      pr_trace_msg(trace_channel, 3, "ill-formed ACL for '%s': %s",
        path, "invalid entry type");
      errno = EACCES;
      return -1;
  }

  /* Iterate through all of the ACL entries, sorting them for later
   * checking.
   */

  acl_pool = make_sub_pool(p);
  acl_groups = make_array(acl_pool, 1, sizeof(aclent_t));
  acl_users = make_array(acl_pool, 1, sizeof(aclent_t));

  for (i = 0; i < nents; i++) {
    if (acls[i].a_type & USER_OBJ) {
      memcpy(&acl_user_entry, &(acls[i]), sizeof(aclent_t));

    } else if (acls[i].a_type & USER) {
      aclent_t *ae_dup = push_array(acl_users);
      memcpy(ae_dup, &(acls[i]), sizeof(aclent_t));

    } else if (acls[i].a_type & GROUP_OBJ) {
      memcpy(&acl_group_entry, &(acls[i]), sizeof(aclent_t));

    } else if (acls[i].a_type & GROUP) {
      aclent_t *ae_dup = push_array(acl_groups);
      memcpy(ae_dup, &(acls[i]), sizeof(aclent_t));

    } else if (acls[i].a_type & OTHER_OBJ) {
      memcpy(&acl_other_entry, &(acls[i]), sizeof(aclent_t));

    } else if (acls[i].a_type & CLASS_OBJ) {
      memcpy(&acl_mask_entry, &(acls[i]), sizeof(aclent_t));
      have_mask_entry = TRUE;
    }
  }

  /* Select the ACL entry that determines access. */
  res = -1;

  /* 1. If the given user ID matches the file owner, use that entry for
   *    access.
   */
  if (uid == st->st_uid) {
    /* Check the acl_user_entry for access. */
    memcpy(&ae, &acl_user_entry, sizeof(aclent_t));
    ae_type = USER_OBJ;
    have_access_entry = TRUE;

    pr_trace_msg(trace_channel, 10, "user ID %lu matches ACL owner user ID",
      (unsigned long) uid);
  }

  /* 2. If not matched above, and f the given user ID matches one of the
   *    named user entries, use that entry for access.
   */
  for (i = 0; !have_access_entry && i < acl_users->nelts; i++) {
    aclent_t e;
    memcpy(&e, &(((aclent_t *) acl_users->elts)[i]), sizeof(aclent_t));

    if (uid == e.a_id) {

      /* Check this entry for access. Note that it'll need to
       * be modified by the mask, if any, later.
       */
      memcpy(&ae, &e, sizeof(aclent_t));
      ae_type = USER;
      have_access_entry = TRUE;

      pr_trace_msg(trace_channel, 10,
        "user ID %lu matches ACL allowed users list", (unsigned long) uid);

      break;
    }
  }

  /* 3. If not matched above, and if one of the group IDs matches the
   *    group owner entry, and the group owner entry contains the
   *    requested permissions, use that entry for access.
   */
  if (!have_access_entry &&
      gid == st->st_gid) {

    /* Check the acl_group_entry for access. First though, we need to
     * see if the acl_group_entry contains the requested permissions.
     */
    if (acl_group_entry.a_perm & mode) {
      memcpy(&ae, &acl_group_entry, sizeof(aclent_t));
      ae_type = GROUP_OBJ;
      have_access_entry = TRUE;

      pr_trace_msg(trace_channel, 10,
        "primary group ID %lu matches ACL owner group ID",
        (unsigned long) gid);
    }
  }

  if (suppl_gids) {
    for (i = 0; !have_access_entry && i < suppl_gids->nelts; i++) {
      gid_t suppl_gid = ((gid_t *) suppl_gids->elts)[i];

      if (suppl_gid == st->st_gid) {
        /* Check the acl_group_entry for access. First though, we need to
         * see if the acl_group_entry contains the requested permissions.
         */
        if (acl_group_entry.a_perm & mode) {
          memcpy(&ae, &acl_group_entry, sizeof(aclent_t));
          ae_type = GROUP_OBJ;
          have_access_entry = TRUE;

          pr_trace_msg(trace_channel, 10,
            "supplemental group ID %lu matches ACL owner group ID",
            (unsigned long) suppl_gid);

          break;
        }
      }
    }
  }

  /* 4. If not matched above, and if one of the group IDs matches one
   *    of the named group entries, and that entry contains the requested
   *    permissions, use that entry for access.
   */
  for (i = 0; !have_access_entry && i < acl_groups->nelts; i++) {
    aclent_t e;
    memcpy(&e, &(((aclent_t *) acl_groups->elts)[i]), sizeof(aclent_t));

    if (gid == e.a_id) {

      /* Check this entry for access. Note that it'll need to
       * be modified by the mask, if any, later.
       */
      if (e.a_perm & mode) {
        memcpy(&ae, &e, sizeof(aclent_t));
        ae_type = GROUP;
        have_access_entry = TRUE;

        pr_trace_msg(trace_channel, 10,
          "primary group ID %lu matches ACL allowed groups list",
          (unsigned long) gid);

        break;
      }
    }

    if (suppl_gids) {
      register unsigned int j;

      for (j = 0; !have_access_entry && j < suppl_gids->nelts; j++) {
        gid_t suppl_gid = ((gid_t *) suppl_gids->elts)[j];

        if (suppl_gid == e.a_id) {
          /* Check this entry for access. Note that it'll need to
           * be modified by the mask, if any, later.
           */
          if (e.a_perm & mode) {
            memcpy(&ae, &e, sizeof(aclent_t));
            ae_type = GROUP;
            have_access_entry = TRUE;

            pr_trace_msg(trace_channel, 10,
              "supplemental group ID %lu matches ACL allowed groups list",
              (unsigned long) suppl_gid);

            break;
          }
        }
      }
    }
  }

  /* 5. If not matched above, and if one of the group IDs matches
   *    the group owner or any of the named group entries, but neither
   *    the group owner entry nor any of the named group entries contains
   *    the requested permissions, access is denied.
   */

  /* XXX implement this condition properly */

  /* 6. If not matched above, the other entry determines access.
   */
  if (!have_access_entry) {
    memcpy(&ae, &acl_other_entry, sizeof(aclent_t));
    ae_type = OTHER_OBJ;
    have_access_entry = TRUE;

    pr_trace_msg(trace_channel, 10, "using ACL 'other' list");
  }

  /* Access determination:
   *
   *  If either the user owner entry or other entry were used, and the
   *  entry contains the requested permissions, access is permitted.
   *
   *  Otherwise, if the selected entry and the mask entry both contain
   *  the requested permissions (or there is no mask entry), access is
   *  permitted.
   *
   *  Otherwise, access is denied.
   */
  switch (ae_type) {
    case USER_OBJ:
    case OTHER_OBJ:
      if (ae.a_perm & mode)
        res = 0;
      break;

    default: 
      if (have_mask_entry) {
        if ((ae.a_perm & mode) &&
            (acl_mask_entry.a_perm & mode))
          res = 0;

      } else {

        /* If there is no mask entry, then access should be granted. */
        if (ae.a_perm & mode)
          res = 0;
      }

      break;
  }

  destroy_pool(acl_pool);

  if (res < 0) {
    errno = EACCES;
    pr_trace_msg(trace_channel, 3,
      "returning EACCES for path '%s', user ID %lu", path,
      (unsigned long) uid);
  }

  return res;
# endif /* HAVE_SOLARIS_POSIX_ACL */
}

# if defined(PR_USE_FACL)

/* FSIO handlers
 */

static int facl_fsio_access(pr_fs_t *fs, const char *path, int mode,
    uid_t uid, gid_t gid, array_header *suppl_gids) {
  int nents = 0;
  struct stat st;
  void *acls;

  pr_fs_clear_cache();
  if (pr_fsio_stat(path, &st) < 0)
    return -1;

  /* Look up the acl for this path. */
# if defined(HAVE_BSD_POSIX_ACL) || defined(HAVE_LINUX_POSIX_ACL)
  acls = acl_get_file(path, ACL_TYPE_ACCESS);

  if (!acls) {
    pr_trace_msg(trace_channel, 5, "unable to retrieve ACL for '%s': [%d] %s",
      path, errno, strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", path);

      /* Fall back to the custom access() function defined in src/fsio.
       * Since that sys_access() function there is not public, we have
       * to duplicate the code.  For now, that is, until a more clean
       * arrangement can be found.
       */
      if (sys_access(fs, path, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", path, strerror(errno));
        return -1;
      }

      return 0;

    } else {
      return -1;
    }
  }

# elif defined(HAVE_SOLARIS_POSIX_ACL)

  nents = acl(path, GETACLCNT, 0, NULL);
  if (nents < 0) {
    pr_trace_msg(trace_channel, 5,
      "unable to retrieve ACL count for '%s': [%d] %s", path, errno,
      strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", path);

      if (sys_access(fs, path, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", path, strerror(errno));
        return -1;
      }   

      return 0;

    } else {
      return -1;
    }
  }

  pr_trace_msg(trace_channel, 10,
    "acl(2) returned %d ACL entries for path '%s'", nents, path);

  acls = pcalloc(fs->fs_pool, nents * sizeof(aclent_t));

  nents = acl(path, GETACL, nents, acls);
  if (nents < 0) {
    pr_trace_msg(trace_channel, 5,
      "unable to retrieve ACL for '%s': [%d] %s", path, errno,
      strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", path);

      if (sys_access(fs, path, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", path, strerror(errno));
        return -1;
      }   

      return 0;

    } else {
      return -1;
    }
  }
# endif

  return check_facl(fs->fs_pool, path, mode, acls, nents, &st,
    uid, gid, suppl_gids);
}

static int facl_fsio_faccess(pr_fh_t *fh, int mode, uid_t uid, gid_t gid,
    array_header *suppl_gids) {
  int nents = 0;
  struct stat st;
  void *acls;

  pr_fs_clear_cache();
  if (pr_fsio_fstat(fh, &st) < 0)
    return -1;

  /* Look up the acl for this fd. */
# if defined(HAVE_BSD_POSIX_ACL) || defined(HAVE_LINUX_POSIX_ACL)
  acls = acl_get_fd(PR_FH_FD(fh));

  if (!acls) {
    pr_trace_msg(trace_channel, 10,
      "unable to retrieve ACL for '%s': [%d] %s", fh->fh_path, errno,
      strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", fh->fh_path);

      /* Fall back to the custom faccess() function defined in src/fsio.
       * Since that sys_faccess() function there is not public, we have
       * to duplicate the code.  For now, that is, until a more clean
       * arrangement can be found.
       */
      if (sys_faccess(fh, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", fh->fh_path, strerror(errno));
        return -1;
      }   

      return 0;

    } else {
      return -1;
    }
  }

# elif defined(HAVE_SOLARIS_POSIX_ACL)

  nents = facl(PR_FH_FD(fh), GETACLCNT, 0, NULL);
  if (nents < 0) {
    pr_trace_msg(trace_channel, 10,
      "unable to retrieve ACL count for '%s': [%d] %s", fh->fh_path,
      errno, strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", fh->fh_path);

      if (sys_faccess(fh, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", fh->fh_path, strerror(errno));
        return -1;
      }   

      return 0;

    } else {
      return -1;
    }
  }

  acls = pcalloc(fh->fh_fs->fs_pool, nents * sizeof(aclent_t));

  nents = facl(PR_FH_FD(fh), GETACL, nents, acls);
  if (nents < 0) {
    pr_trace_msg(trace_channel, 10,
      "unable to retrieve ACL for '%s': [%d] %s", fh->fh_path, errno,
      strerror(errno));

    if (is_errno_eperm()) {
      pr_trace_msg(trace_channel, 3, "ACL retrieval operation not supported "
        "for '%s', falling back to normal access check", fh->fh_path);

      if (sys_faccess(fh, mode, uid, gid, suppl_gids) < 0) {
        pr_trace_msg(trace_channel, 6, "normal access check for '%s' "
          "failed: %s", fh->fh_path, strerror(errno));
        return -1;
      }   

      return 0;

    } else {
      return -1;
    }
  }
# endif

  return check_facl(fh->fh_fs->fs_pool, fh->fh_path, mode, acls, nents, &st,
    uid, gid, suppl_gids);
}
# endif /* !PR_USE_FACL */

#endif /* HAVE_POSIX_ACL */

#if defined(PR_SHARED_MODULE)
static void facl_mod_unload_ev(const void *event_data, void *user_data) {
  if (strcmp("mod_facl.c", (const char *) event_data) == 0) {
    pr_event_unregister(&facl_module, NULL, NULL);
    pr_unregister_fs("facl");
  }
}
#endif /* !PR_SHARED_MODULE */

/* Initialization routines
 */

static int facl_init(void) {
#if defined(PR_USE_FACL) && defined(HAVE_POSIX_ACL)
  pr_fs_t *fs;
#endif /* PR_USE_FACL and HAVE_POSIX_ACL */

#if defined(PR_SHARED_MODULE)
  pr_event_register(&facl_module, "core.module-unload", facl_mod_unload_ev,
    NULL);
#endif /* PR_SHARED_MODULE */

  if (!facl_engine)
    return 0;

#if defined(PR_USE_FACL) && defined(HAVE_POSIX_ACL)
  fs = pr_register_fs(permanent_pool, "facl", "/");
  if (!fs) {
    pr_log_pri(PR_LOG_ERR, MOD_FACL_VERSION ": error registering 'facl' FS: %s",
      strerror(errno));
    return -1;
  }
  pr_log_debug(DEBUG6, MOD_FACL_VERSION ": registered 'facl' FS");

  /* Ensure that our ACL-checking handlers are used. */
  fs->access = facl_fsio_access;
  fs->faccess = facl_fsio_faccess;

# if defined(PR_SHARED_MODULE)
    pr_event_register(&facl_module, "core.module-unload", facl_mod_unload_ev,
      NULL);
# endif /* !PR_SHARED_MODULE */
#endif /* PR_USE_FACL and HAVE_POSIX_ACL */

  return 0;
}

/* Configuration handlers
 */

/* usage: FACLEngine on|off */
MODRET set_faclengine(cmd_rec *cmd) {
  int bool = -1;

  CHECK_ARGS(cmd, 1);
  CHECK_CONF(cmd, CONF_ROOT);

  bool = get_boolean(cmd, 1);
  if (bool == -1)
    CONF_ERROR(cmd, "expected Boolean parameter");

  facl_engine = bool;
  return PR_HANDLED(cmd);
}

/* Module Tables
 */

static conftable facl_conftab[] = {
  { "FACLEngine",		set_faclengine,		NULL },
  { NULL }
};

module facl_module = {
  /* Always NULL */
  NULL, NULL,

  /* Module API version */
  0x20,

  /* Module name */
  "facl",

  /* Module configuration directive handlers */
  facl_conftab,

  /* Module command handlers */
  NULL,

  /* Module authentication handlers */
  NULL,

  /* Module initialization */
  facl_init,

  /* Session initialization */
  NULL,

  /* Module version */
  MOD_FACL_VERSION
};

