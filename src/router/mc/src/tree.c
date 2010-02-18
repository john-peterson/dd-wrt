/* Directory tree browser for the Midnight Commander
   Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
   2003, 2004, 2005, 2007 Free Software Foundation, Inc.

   Written: 1994, 1996 Janne Kukonlehto
            1997 Norbert Warmuth
            1996, 1999 Miguel de Icaza

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   This module has been converted to be a widget.

   The program load and saves the tree each time the tree widget is
   created and destroyed.  This is required for the future vfs layer,
   it will be possible to have tree views over virtual file systems.

   */
#include <config.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "tty.h"
#include "color.h"
#include "wtools.h"	/* message() */
#include "dir.h"
#include "dialog.h"
#include "widget.h"
#include "panel.h"
#include "mouse.h"
#include "main.h"
#include "file.h"	/* For copy_dir_dir(), move_dir_dir(), erase_dir() */
#include "help.h"
#include "key.h"	/* For mi_getch() */
#include "tree.h"
#include "treestore.h"
#include "cmd.h"
#include "history.h"

#define tlines(t) (t->is_panel ? t->widget.lines-2 - (show_mini_info ? 2 : 0) : t->widget.lines)

extern int command_prompt;

/* Use the color of the parent widget for the unselected entries */
#define TREE_NORMALC(h) (DLG_NORMALC (h))

/* Specifies the display mode: 1d or 2d */
static int tree_navigation_flag;

struct WTree {
    Widget widget;
    struct TreeStore *store;
    tree_entry *selected_ptr;	/* The selected directory */
    char search_buffer[256];	/* Current search string */
    tree_entry **tree_shown;	/* Entries currently on screen */
    int is_panel;		/* panel or plain widget flag */
    int active;			/* if it's currently selected */
    int searching;		/* Are we on searching mode? */
    int topdiff;		/* The difference between the topmost
				   shown and the selected */
};

/* Forwards */
static void save_tree (WTree *tree);
static void tree_rescan_cmd (WTree *);

static tree_entry *back_ptr (tree_entry *ptr, int *count)
{
    int i = 0;

    while (ptr && ptr->prev && i < *count){
	ptr = ptr->prev;
	i ++;
    }
    *count = i;
    return ptr;
}

static tree_entry *forw_ptr (tree_entry *ptr, int *count)
{
    int i = 0;

    while (ptr && ptr->next && i < *count){
	ptr = ptr->next;
	i ++;
    }
    *count = i;
    return ptr;
}

static void
remove_callback (tree_entry *entry, void *data)
{
	WTree *tree = data;

	if (tree->selected_ptr == entry){
		if (tree->selected_ptr->next)
			tree->selected_ptr = tree->selected_ptr->next;
		else
			tree->selected_ptr = tree->selected_ptr->prev;
	}
}

static void tree_remove_entry (WTree *tree, char *name)
{
    (void) tree;
    tree_store_remove_entry (name);
}

static void tree_destroy (WTree *tree)
{
    tree_store_remove_entry_remove_hook (remove_callback);
    save_tree (tree);

    g_free (tree->tree_shown);
    tree->tree_shown = 0;
    tree->selected_ptr = NULL;
}

/* Loads the .mc.tree file */
static void load_tree (WTree *tree)
{
    tree_store_load ();

    tree->selected_ptr = tree->store->tree_first;
    tree_chdir (tree, home_dir);
}

/* Save the .mc.tree file */
static void save_tree (WTree *tree)
{
    int error;

    (void) tree;
    error = tree_store_save ();

    if (error){
	fprintf (stderr, _("Cannot open the %s file for writing:\n%s\n"), MC_TREE,
		 unix_error_string (error));
    }
}

static void tree_show_mini_info (WTree *tree, int tree_lines, int tree_cols)
{
    Dlg_head *h = tree->widget.parent;
    int      line;

    /* Show mini info */
    if (tree->is_panel){
	if (!show_mini_info)
	    return;
	line = tree_lines+2;
    } else
	line = tree_lines+1;

    widget_move (&tree->widget, line, 1);
    hline (' ', tree_cols);
    widget_move (&tree->widget, line, 1);

    if (tree->searching){
	/* Show search string */
	attrset (TREE_NORMALC (h));
	attrset (DLG_FOCUSC (h));
	addch (PATH_SEP);

	addstr ((char *) name_trunc (tree->search_buffer, tree_cols-2));
	addch (' ');
	attrset (DLG_FOCUSC (h));
    } else {
	/* Show full name of selected directory */
	addstr ((char *) name_trunc (tree->selected_ptr->name, tree_cols));
    }
}

static void show_tree (WTree *tree)
{
    Dlg_head *h = tree->widget.parent;
    tree_entry *current;
    int i, j, topsublevel;
    int x, y;
    int tree_lines, tree_cols;

    /* Initialize */
    x = y = 0;
    tree_lines = tlines (tree);
    tree_cols  = tree->widget.cols;

    attrset (TREE_NORMALC (h));
    widget_move ((Widget*)tree, y, x);
    if (tree->is_panel){
	tree_cols  -= 2;
	x = y = 1;
    }

    g_free (tree->tree_shown);
    tree->tree_shown = g_new (tree_entry*, tree_lines);

    for (i = 0; i < tree_lines; i++)
	tree->tree_shown [i] = NULL;
    if (tree->store->tree_first)
	topsublevel = tree->store->tree_first->sublevel;
    else
	topsublevel = 0;
    if (!tree->selected_ptr){
	tree->selected_ptr = tree->store->tree_first;
	tree->topdiff = 0;
    }
    current = tree->selected_ptr;

    /* Calculate the directory which is to be shown on the topmost line */
    if (tree_navigation_flag){
	i = 0;
	while (current->prev && i < tree->topdiff){
	    current = current->prev;
	    if (current->sublevel < tree->selected_ptr->sublevel){
		if (strncmp (current->name, tree->selected_ptr->name,
			     strlen (current->name)) == 0)
		    i++;
	    } else if (current->sublevel == tree->selected_ptr->sublevel){
		for (j = strlen (current->name) - 1; current->name [j] != PATH_SEP; j--);
		if (strncmp (current->name, tree->selected_ptr->name, j) == 0)
		    i++;
	    } else if (current->sublevel == tree->selected_ptr->sublevel + 1
		       && strlen (tree->selected_ptr->name) > 1){
		if (strncmp (current->name, tree->selected_ptr->name,
			     strlen (tree->selected_ptr->name)) == 0)
		    i++;
	    }
	}
	tree->topdiff = i;
    } else
	current = back_ptr (current, &tree->topdiff);

    /* Loop for every line */
    for (i = 0; i < tree_lines; i++){
	/* Move to the beginning of the line */
	widget_move (&tree->widget, y+i, x);

	hline (' ', tree_cols);
	widget_move (&tree->widget, y+i, x);

	if (!current)
	    continue;

	tree->tree_shown [i] = current;
	if (current->sublevel == topsublevel){

	    /* Top level directory */
	    if (tree->active && current == tree->selected_ptr) {
		if (!use_colors && !tree->is_panel)
			attrset (MARKED_COLOR);
		else
			attrset (SELECTED_COLOR);
	    }

	    /* Show full name */
	    addstr ((char *) name_trunc (current->name, tree_cols - 6));
	} else{
	    /* Sub level directory */

	    acs ();
	    /* Output branch parts */
	    for (j = 0; j < current->sublevel - topsublevel - 1; j++){
		if (tree_cols - 8 - 3 * j < 9)
		    break;
		addch (' ');
		if (current->submask & (1 << (j + topsublevel + 1)))
		    addch (ACS_VLINE);
		else
		    addch (' ');
		addch (' ');
	    }
	    addch (' '); j++;
	    if (!current->next || !(current->next->submask & (1 << current->sublevel)))
		addch (ACS_LLCORNER);
	    else
		addch (ACS_LTEE);
	    addch (ACS_HLINE);
	    noacs ();

	    if (tree->active && current == tree->selected_ptr) {
		/* Selected directory -> change color */
		if (!use_colors && !tree->is_panel)
		    attrset (MARKED_COLOR);
		else
		    attrset (SELECTED_COLOR);
	    }

	    /* Show sub-name */
	    addch (' ');
	    addstr ((char *) name_trunc (current->subname,
				tree_cols - 2 - 4 - 3 * j));
	}
	addch (' ');

	/* Return to normal color */
	attrset (TREE_NORMALC (h));

	/* Calculate the next value for current */
	current = current->next;
	if (tree_navigation_flag){
	    while (current){
		if (current->sublevel < tree->selected_ptr->sublevel){
		    if (strncmp (current->name, tree->selected_ptr->name,
				 strlen (current->name)) == 0)
			break;
		} else if (current->sublevel == tree->selected_ptr->sublevel){
		    for (j = strlen (current->name) - 1; current->name [j] != PATH_SEP; j--);
		    if (strncmp (current->name,tree->selected_ptr->name,j)== 0)
			break;
		} else if (current->sublevel == tree->selected_ptr->sublevel+1
			   && strlen (tree->selected_ptr->name) > 1){
		    if (strncmp (current->name, tree->selected_ptr->name,
				 strlen (tree->selected_ptr->name)) == 0)
			break;
		}
		current = current->next;
	    }
	}
    }
    tree_show_mini_info (tree, tree_lines, tree_cols);
}

static void check_focus (WTree *tree)
{
    if (tree->topdiff < 3)
	tree->topdiff = 3;
    else if (tree->topdiff >= tlines (tree) - 3)
	tree->topdiff = tlines (tree) - 3 - 1;
}

static void tree_move_backward (WTree *tree, int i)
{
    tree_entry *current;
    int j = 0;

    if (tree_navigation_flag){
	current = tree->selected_ptr;
	while (j < i && current->prev
	       && current->prev->sublevel >= tree->selected_ptr->sublevel){
	    current = current->prev;
	    if (current->sublevel == tree->selected_ptr->sublevel){
		tree->selected_ptr = current;
		j ++;
	    }
	}
	i = j;
    } else
	tree->selected_ptr = back_ptr (tree->selected_ptr, &i);
    tree->topdiff -= i;
    check_focus (tree);
}

static void tree_move_forward (WTree *tree, int i)
{
    tree_entry *current;
    int j = 0;

    if (tree_navigation_flag){
	current = tree->selected_ptr;
	while (j < i && current->next
	       && current->next->sublevel >= tree->selected_ptr->sublevel){
	    current = current->next;
	    if (current->sublevel == tree->selected_ptr->sublevel){
		tree->selected_ptr = current;
		j ++;
	    }
	}
	i = j;
    } else
	tree->selected_ptr = forw_ptr (tree->selected_ptr, &i);
    tree->topdiff += i;
    check_focus (tree);
}

static void tree_move_to_child (WTree *tree)
{
    tree_entry *current;

    /* Do we have a starting point? */
    if (!tree->selected_ptr)
	return;
    /* Take the next entry */
    current = tree->selected_ptr->next;
    /* Is it the child of the selected entry */
    if (current && current->sublevel > tree->selected_ptr->sublevel){
	/* Yes -> select this entry */
	tree->selected_ptr = current;
	tree->topdiff++;
	check_focus (tree);
    } else {
	/* No -> rescan and try again */
	tree_rescan_cmd (tree);
	current = tree->selected_ptr->next;
	if (current && current->sublevel > tree->selected_ptr->sublevel){
	    tree->selected_ptr = current;
	    tree->topdiff++;
	    check_focus (tree);
	}
    }
}

static int tree_move_to_parent (WTree *tree)
{
    tree_entry *current;
    tree_entry *old;

    if (!tree->selected_ptr)
	return 0;
    old = tree->selected_ptr;
    current = tree->selected_ptr->prev;
    while (current && current->sublevel >= tree->selected_ptr->sublevel){
	current = current->prev;
	tree->topdiff--;
    }
    if (!current)
	current = tree->store->tree_first;
    tree->selected_ptr = current;
    check_focus (tree);
    return tree->selected_ptr != old;
}

static void tree_move_to_top (WTree *tree)
{
    tree->selected_ptr = tree->store->tree_first;
    tree->topdiff = 0;
}

static void tree_move_to_bottom (WTree *tree)
{
    tree->selected_ptr = tree->store->tree_last;
    tree->topdiff = tlines (tree) - 3 - 1;
}

void tree_chdir (WTree *tree, const char *dir)
{
    tree_entry *current;

    current = tree_store_whereis (dir);
    if (current){
	tree->selected_ptr = current;
	check_focus (tree);
    }
}

void
sync_tree (const char *path)
{
    tree_chdir (the_tree, path);
}

/* Handle mouse click */
static void
tree_event (WTree *tree, int y)
{
    if (tree->tree_shown [y]){
	tree->selected_ptr = tree->tree_shown [y];
	tree->topdiff = y;
    }
    show_tree (tree);
}

static void chdir_sel (WTree *tree);

static void maybe_chdir (WTree *tree)
{
    if (!(xtree_mode && tree->is_panel))
	return;
    if (is_idle ())
	chdir_sel (tree);
}

/* Mouse callback */
static int
event_callback (Gpm_Event *event, void *data)
{
    WTree *tree = data;

    if (!(event->type & GPM_UP))
	return MOU_NORMAL;

    if (tree->is_panel)
	event->y--;

    event->y--;

    if (!tree->active)
	change_panel ();

    if (event->y < 0){
	tree_move_backward (tree, tlines (tree) - 1);
	show_tree (tree);
    }
    else if (event->y >= tlines (tree)){
	tree_move_forward (tree, tlines (tree) - 1);
	show_tree (tree);
    } else {
	tree_event (tree, event->y);
	if ((event->type & (GPM_UP|GPM_DOUBLE)) == (GPM_UP|GPM_DOUBLE)){
	    chdir_sel (tree);
	}
    }
    return MOU_NORMAL;
}

/* Search tree for text */
static int search_tree (WTree *tree, char *text)
{
    tree_entry *current;
    int len;
    int wrapped = 0;
    int found = 0;

    len = strlen (text);
    current = tree->selected_ptr;
    found = 0;
    while (!wrapped || current != tree->selected_ptr){
	if (strncmp (current->subname, text, len) == 0){
	    tree->selected_ptr = current;
	    found = 1;
	    break;
	}
	current = current->next;
	if (!current){
	    current = tree->store->tree_first;
	    wrapped = 1;
	}
	tree->topdiff++;
    }
    check_focus (tree);
    return found;
}

static void tree_do_search (WTree *tree, int key)
{
    size_t l;

    l = strlen (tree->search_buffer);
    if (l && (key == KEY_BACKSPACE))
	tree->search_buffer [--l] = 0;
    else {
	if (key && l < sizeof (tree->search_buffer)){
	    tree->search_buffer [l] = key;
	    tree->search_buffer [l+1] = 0;
	    l++;
	}
    }

    if (!search_tree (tree, tree->search_buffer))
	tree->search_buffer [--l] = 0;

    show_tree (tree);
    maybe_chdir (tree);
}

static void
tree_rescan_cmd (WTree *tree)
{
    char old_dir [MC_MAXPATHLEN];

    if (!tree->selected_ptr || !mc_get_current_wd (old_dir, MC_MAXPATHLEN) ||
	mc_chdir (tree->selected_ptr->name))
	return;

    tree_store_rescan (tree->selected_ptr->name);
    mc_chdir (old_dir);
}

static void
tree_forget_cmd (void *data)
{
    WTree *tree = data;
    if (tree->selected_ptr)
	tree_remove_entry (tree, tree->selected_ptr->name);
}

static void tree_copy (WTree *tree, const char *default_dest)
{
    char   *dest;
    off_t  count = 0;
    double bytes = 0;
    FileOpContext *ctx;

    if (!tree->selected_ptr)
	return;
    g_snprintf (cmd_buf, sizeof(cmd_buf), _("Copy \"%s\" directory to:"),
	     name_trunc (tree->selected_ptr->name, 50));
    dest = input_expand_dialog (_(" Copy "), cmd_buf, MC_HISTORY_FM_TREE_COPY, default_dest);

    if (!dest)
	return;

    if (!*dest){
	g_free (dest);
	return;
    }

    ctx = file_op_context_new (OP_COPY);
    file_op_context_create_ui (ctx, FALSE);
    copy_dir_dir (ctx, tree->selected_ptr->name, dest, 1, 0, 0, 0, &count, &bytes);
    file_op_context_destroy (ctx);

    g_free (dest);
}

static void
tree_help_cmd (void)
{
    interactive_display (NULL,  "[Directory Tree]");
}

static void
tree_copy_cmd (void *data)
{
    WTree *tree = data;
    tree_copy (tree, "");
}

static void tree_move (WTree *tree, const char *default_dest)
{
    char   *dest;
    struct stat buf;
    double bytes = 0;
    off_t  count = 0;
    FileOpContext *ctx;

    if (!tree->selected_ptr)
	return;
    g_snprintf (cmd_buf, sizeof (cmd_buf), _("Move \"%s\" directory to:"),
	     name_trunc (tree->selected_ptr->name, 50));
    dest = input_expand_dialog (_(" Move "), cmd_buf, MC_HISTORY_FM_TREE_MOVE, default_dest);
    if (!dest)
	return;
    if (!*dest){
	g_free (dest);
	return;
    }
    if (stat (dest, &buf)){
	message (1, MSG_ERROR, _(" Cannot stat the destination \n %s "),
		 unix_error_string (errno));
	g_free (dest);
	return;
    }
    if (!S_ISDIR (buf.st_mode)){
	file_error (_(" Destination \"%s\" must be a directory \n %s "),
		    dest);
	g_free (dest);
	return;
    }

    ctx = file_op_context_new (OP_MOVE);
    file_op_context_create_ui (ctx, FALSE);
    move_dir_dir (ctx, tree->selected_ptr->name, dest, &count, &bytes);
    file_op_context_destroy (ctx);

    g_free (dest);
}

static void
tree_move_cmd (void *data)
{
    WTree *tree = data;
    tree_move (tree, "");
}

#if 0
static void
tree_mkdir_cmd (WTree *tree)
{
    char old_dir [MC_MAXPATHLEN];

    if (!tree->selected_ptr)
	return;
    if (!mc_get_current_wd (old_dir, MC_MAXPATHLEN))
	return;
    if (chdir (tree->selected_ptr->name))
	return;
    /* FIXME
    mkdir_cmd (tree);
    */
    tree_rescan_cmd (tree);
    chdir (old_dir);
}
#endif

static void
tree_rmdir_cmd (WTree *tree)
{
    off_t count = 0;
    double bytes = 0;
    FileOpContext *ctx;

    if (!tree->selected_ptr)
	return;

    if (confirm_delete) {
	char *buf;
	int result;

	buf =
	    g_strdup_printf (_("  Delete %s?  "),
			     tree->selected_ptr->name);
	result =
	    query_dialog (_(" Delete "), buf, 3, 2, _("&Yes"), _("&No"));
	g_free (buf);
	if (result != 0)
	    return;
    }

    ctx = file_op_context_new (OP_DELETE);
    file_op_context_create_ui (ctx, FALSE);
    if (erase_dir (ctx, tree->selected_ptr->name, &count, &bytes) ==
	FILE_CONT)
	tree_forget_cmd (tree);
    file_op_context_destroy (ctx);
}

static void set_navig_label (WTree *tree);

static void
tree_toggle_navig (void *data)
{
    WTree *tree = data;
    /* FIXME: invalid use of boolean variable */
    tree_navigation_flag = 1 - tree_navigation_flag;
    set_navig_label (tree);
}

static void
set_navig_label (WTree *tree)
{
    buttonbar_set_label_data (tree->widget.parent, 4,
		       tree_navigation_flag ? _("Static") : _("Dynamc"),
		       tree_toggle_navig, tree);
}

static void
move_down (WTree *tree)
{
    tree_move_forward (tree, 1);
    show_tree (tree);
    maybe_chdir (tree);
}

static void
move_up (WTree *tree)
{
    tree_move_backward (tree, 1);
    show_tree (tree);
    maybe_chdir (tree);
}

static void
move_home (WTree *tree)
{
    tree_move_to_top (tree);
    show_tree (tree);
    maybe_chdir (tree);
}

static void
move_end (WTree *tree)
{
    tree_move_to_bottom (tree);
    show_tree (tree);
    maybe_chdir (tree);
}

static int
move_left (WTree *tree)
{
    int v;

    if (tree_navigation_flag){
	v = tree_move_to_parent (tree);
	show_tree (tree);
	maybe_chdir (tree);
	return v;
    }
    return 0;
}

static int
move_right (WTree *tree)
{
    if (tree_navigation_flag){
	tree_move_to_child (tree);
	show_tree (tree);
	maybe_chdir (tree);
	return 1;
    }
    return 0;
}

static void
move_prevp (WTree *tree)
{
    tree_move_backward (tree, tlines (tree) - 1);
    show_tree (tree);
    maybe_chdir (tree);
}

static void
move_nextp (WTree *tree)
{
    tree_move_forward (tree, tlines (tree) - 1);
    show_tree (tree);
    maybe_chdir (tree);
}

static void
chdir_sel (WTree *tree)
{
    if (!tree->is_panel) {
	return;
    }
    change_panel ();
    if (do_cd (tree->selected_ptr->name, cd_exact)) {
	select_item (current_panel);
    } else {
	message (1, MSG_ERROR, _(" Cannot chdir to \"%s\" \n %s "),
		 tree->selected_ptr->name, unix_error_string (errno));
    }
    change_panel ();
    show_tree (tree);
    return;
}

static void
tree_start_search (WTree *tree)
{
    int i;

    if (tree->searching){

	if (tree->selected_ptr == tree->store->tree_last)
	    tree_move_to_top(tree);
	else {
	/* set navigation mode temporarily to 'Static' because in
	 * dynamic navigation mode tree_move_forward will not move
	 * to a lower sublevel if necessary (sequent searches must
	 * start with the directory followed the last found directory)
         */
	    i = tree_navigation_flag;
	    tree_navigation_flag = 0;
	    tree_move_forward (tree, 1);
	    tree_navigation_flag = i;
	}
	tree_do_search (tree, 0);
    }
    else {
	tree->searching = 1;
	tree->search_buffer[0] = 0;
    }
}

typedef void (*tree_key_action) (WTree *);
typedef struct {
    int key_code;
    tree_key_action fn;
} tree_key_map;

static const tree_key_map tree_keymap [] = {
    { XCTRL('n'), move_down    },
    { XCTRL('p'), move_up      },
    { KEY_DOWN,   move_down    },
    { KEY_UP,     move_up      },
    { '\n',       chdir_sel    },
    { KEY_ENTER,  chdir_sel    },
    { KEY_HOME,   move_home    },
    { KEY_A1,     move_home    },
    { ALT ('<'),  move_home    },
    { KEY_END,    move_end     },
    { KEY_C1,     move_end     },
    { ALT ('>'),  move_end     },
    { KEY_NPAGE,  move_nextp   },
    { KEY_PPAGE,  move_prevp   },
    { XCTRL('v'), move_nextp   },
    { ALT('v'),   move_prevp   },
    { XCTRL('p'), move_up      },
    { XCTRL('p'), move_down    },
    { XCTRL('s'), tree_start_search },
    { ALT('s'),   tree_start_search },
    { XCTRL('r'), tree_rescan_cmd },
    { KEY_DC,     tree_rmdir_cmd },
    { 0, 0 }
    };

static inline cb_ret_t
tree_key (WTree *tree, int key)
{
    int i;

    for (i = 0; tree_keymap [i].key_code; i++){
	if (key == tree_keymap [i].key_code){
	    if (tree_keymap [i].fn != tree_start_search)
	        tree->searching = 0;
	    (*tree_keymap [i].fn)(tree);
	    show_tree (tree);
	    return MSG_HANDLED;
	}
    }

    /* We do not want to use them if we do not need to */
    /* Input line may want to take the motion key event */
    if (key == KEY_LEFT)
	return move_left (tree) ? MSG_HANDLED : MSG_NOT_HANDLED;

    if (key == KEY_RIGHT)
	return move_right (tree) ? MSG_HANDLED : MSG_NOT_HANDLED;

    if (is_abort_char (key)) {
	if (tree->is_panel) {
	    tree->searching = 0;
	    show_tree (tree);
	    return MSG_HANDLED;  /* eat abort char */
	}
	/* modal tree dialog: let upper layer see the
	   abort character and close the dialog */
	return MSG_NOT_HANDLED;
    }

    /* Do not eat characters not meant for the tree below ' ' (e.g. C-l). */
    if ((key >= ' ' && key <= 255) || key == KEY_BACKSPACE) {
	if (tree->searching){
	    tree_do_search (tree, key);
	    show_tree (tree);
	    return MSG_HANDLED;
	}

	if (!command_prompt) {
	    tree_start_search (tree);
	    tree_do_search (tree, key);
	    return MSG_HANDLED;
	}
	return tree->is_panel ? MSG_HANDLED : MSG_NOT_HANDLED;
    }

    return MSG_NOT_HANDLED;
}

static void
tree_frame (Dlg_head *h, WTree *tree)
{
    attrset (NORMAL_COLOR);
    widget_erase ((Widget*) tree);
    if (tree->is_panel)
	draw_double_box (h, tree->widget.y, tree->widget.x, tree->widget.lines,
		         tree->widget.cols);

    if (show_mini_info && tree->is_panel){
	widget_move (tree, tlines (tree) + 1, 1);
	hline (ACS_HLINE, tree->widget.cols - 2);
    }
}

static void
tree_rescan_command (void *data)
{
    WTree *tree = data;
    tree_rescan_cmd (tree);
}

static void
tree_rmdir_command (void *data)
{
    WTree *tree = data;
    tree_rmdir_cmd (tree);
}

static cb_ret_t
tree_callback (Widget *w, widget_msg_t msg, int parm)
{
    WTree *tree = (WTree *) w;
    Dlg_head *h = tree->widget.parent;

    switch (msg) {
    case WIDGET_DRAW:
	tree_frame (h, tree);
	show_tree (tree);
	return MSG_HANDLED;

    case WIDGET_KEY:
	return tree_key (tree, parm);

    case WIDGET_FOCUS:
	tree->active = 1;
	buttonbar_set_label (h, 1, _("Help"), tree_help_cmd);
	buttonbar_set_label_data (h, 2, _("Rescan"),
	    tree_rescan_command, tree);
	buttonbar_set_label_data (h, 3, _("Forget"), tree_forget_cmd, tree);
	buttonbar_set_label_data (h, 5, _("Copy"), tree_copy_cmd, tree);
	buttonbar_set_label_data (h, 6, _("RenMov"), tree_move_cmd, tree);
#if 0
	/* FIXME: mkdir is currently defunct */
	buttonbar_set_label_data (h, 7, _("Mkdir"), tree_mkdir_cmd, tree);
#else
	buttonbar_clear_label (h, 7);
#endif
	buttonbar_set_label_data (h, 8, _("Rmdir"), tree_rmdir_command, tree);
	set_navig_label (tree);
	buttonbar_redraw (h);


	/* FIXME: Should find a better way of only displaying the
	   currently selected item */
	show_tree (tree);
	return MSG_HANDLED;

	/* FIXME: Should find a better way of changing the color of the
	   selected item */

    case WIDGET_UNFOCUS:
	tree->active = 0;
	show_tree (tree);
	return MSG_HANDLED;

    case WIDGET_DESTROY:
	tree_destroy (tree);
	return MSG_HANDLED;

    default:
	return default_proc (msg, parm);
    }
}

WTree *
tree_new (int is_panel, int y, int x, int lines, int cols)
{
    WTree *tree = g_new (WTree, 1);

    init_widget (&tree->widget, y, x, lines, cols,
		 tree_callback, event_callback);
    tree->is_panel = is_panel;
    tree->selected_ptr = 0;

    tree->store = tree_store_get ();
    tree_store_add_entry_remove_hook (remove_callback, tree);
    tree->tree_shown = 0;
    tree->search_buffer[0] = 0;
    tree->topdiff = tree->widget.lines / 2;
    tree->searching = 0;
    tree->active = 0;

    /* We do not want to keep the cursor */
    widget_want_cursor (tree->widget, 0);
    load_tree (tree);
    return tree;
}

/* Return name of the currently selected entry */
char *
tree_selected_name (WTree *tree)
{
    return tree->selected_ptr->name;
}

