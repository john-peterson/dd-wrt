/*
   lib/vfs - manipulations with temp files and  dirs

   Copyright (C) 2012
   The Free Software Foundation, Inc.

   Written by:
   Slava Zanko <slavazanko@gmail.com>, 2012

   This file is part of the Midnight Commander.

   The Midnight Commander is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   The Midnight Commander is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define TEST_SUITE_NAME "/lib/vfs"

#include <config.h>

#include <check.h>

#include "lib/global.c"

#ifndef HAVE_CHARSET
#define HAVE_CHARSET 1
#endif

#include "lib/charsets.h"

#include "lib/strutil.h"
#include "lib/vfs/xdirentry.h"
#include "lib/vfs/path.h"

#include "src/vfs/local/local.c"


struct vfs_s_subclass test_subclass1, test_subclass2, test_subclass3;
struct vfs_class vfs_test_ops1, vfs_test_ops2, vfs_test_ops3;

static void
setup (void)
{
    str_init_strings (NULL);

    vfs_init ();
    init_localfs ();
    vfs_setup_work_dir ();
}

static void
teardown (void)
{
    vfs_shut ();
    str_uninit_strings ();
}

/* --------------------------------------------------------------------------------------------- */
START_TEST (test_mc_tmpdir)
{
    const char *tmpdir;
    const char *env_tmpdir;

    tmpdir = mc_tmpdir();
    fail_unless (
        g_file_test (tmpdir, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR),
        "\nNo such directory: %s\n", tmpdir
    );

    env_tmpdir = g_getenv ("MC_TMPDIR");
    fail_unless (
        strcmp (env_tmpdir, tmpdir) == 0,
        "\nenv_tmpdir=%s\n    tmpdir=%s\n", env_tmpdir, tmpdir
    );
}
END_TEST

START_TEST (test_mc_mkstemps)
{
    vfs_path_t *pname_vpath = NULL;
    char *pname;
    char *begin_pname;
    int fd;

    fd = mc_mkstemps (&pname_vpath, "mctest-", NULL);
    if (fd == -1)
    {
        fail ("\nerror creating temp file!\n");
    }
    pname = vfs_path_to_str (pname_vpath);
    vfs_path_free (pname_vpath);
    close (fd);

    fail_unless (
        g_file_test (pname, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR),
        "\nNo such file: %s\n", pname
    );
    unlink(pname);

    begin_pname = g_build_filename (mc_tmpdir(), "mctest-", NULL);
    fail_unless (
        strncmp(pname, begin_pname, strlen(begin_pname)) == 0,
        "\nstart of %s should be equal to %s\n", pname, begin_pname
    );
    g_free (pname);
}
END_TEST

/* --------------------------------------------------------------------------------------------- */

int
main (void)
{
    int number_failed;

    Suite *s = suite_create (TEST_SUITE_NAME);
    TCase *tc_core = tcase_create ("Core");
    SRunner *sr;

    tcase_add_checked_fixture (tc_core, setup, teardown);

    /* Add new tests here: *************** */
    tcase_add_test (tc_core, test_mc_tmpdir);
    tcase_add_test (tc_core, test_mc_mkstemps);
    /* *********************************** */

    suite_add_tcase (s, tc_core);
    sr = srunner_create (s);
    srunner_set_log (sr, "tempdir.log");
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? 0 : 1;
}

/* --------------------------------------------------------------------------------------------- */
