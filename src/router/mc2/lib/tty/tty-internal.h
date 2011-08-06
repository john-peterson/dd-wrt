
/** \file tty-internal.h
 *  \brief Header: internal suff of the terminal controlling library
 */

#ifndef MC__TTY_INTERNAL_H
#define MC__TTY_INTERNAL_H

#include "lib/global.h"         /* include <glib.h> */

/*** typedefs(not structures) and defined constants **********************************************/

/* Taken from S-Lang's slutty.c */
#ifdef ultrix                   /* Ultrix gets _POSIX_VDISABLE wrong! */
#define NULL_VALUE -1
#else
#ifdef _POSIX_VDISABLE
#define NULL_VALUE _POSIX_VDISABLE
#else
#define NULL_VALUE 255
#endif
#endif

/*** enums ***************************************************************************************/

/*** structures declarations (and typedefs of structures)*****************************************/

/*** global variables defined in .c file *********************************************************/

/* If true lines are shown by spaces */
extern gboolean slow_tty;

/* If true use +, -, | for line drawing */
extern gboolean ugly_line_drawing;

/* The mouse is currently: TRUE - enabled, FALSE - disabled */
extern gboolean mouse_enabled;

/*** declarations of public functions ************************************************************/

char *mc_tty_normalize_from_utf8 (const char *);

/*** inline functions ****************************************************************************/
#endif /* MC_TTY_INTERNAL_H */
