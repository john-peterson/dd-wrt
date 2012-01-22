/** \file layout.h
 *  \brief Header: panel layout module
 */

#ifndef MC__LAYOUT_H
#define MC__LAYOUT_H

#include "lib/global.h"
#include "lib/widget.h"

/*** typedefs(not structures) and defined constants **********************************************/

typedef enum
{
    view_listing = 0,           /* Directory listing */
    view_info = 1,              /* Information panel */
    view_tree = 2,              /* Tree view */
    view_quick = 3,             /* Quick view */
    view_nothing = 4,           /* Undefined */
} panel_view_mode_t;

/*** enums ***************************************************************************************/

/*** structures declarations (and typedefs of structures)*****************************************/

struct WPanel;

/*** global variables defined in .c file *********************************************************/

extern int equal_split;
extern int first_panel_size;
extern int output_lines;
extern gboolean command_prompt;
extern int menubar_visible;
extern int output_start_y;
extern gboolean xterm_title;
extern int free_space;

extern int horizontal_split;
extern int nice_rotating_dash;

/*** declarations of public functions ************************************************************/

void layout_change (void);
void layout_box (void);
void setup_panels (void);
void destroy_panels (void);
void setup_cmdline (void);
void set_display_type (int num, panel_view_mode_t type);
void swap_panels (void);
panel_view_mode_t get_display_type (int idx);
panel_view_mode_t get_current_type (void);
panel_view_mode_t get_other_type (void);
int get_current_index (void);
int get_other_index (void);
const char *get_nth_panel_name (int num);

struct Widget *get_panel_widget (int idx);

struct WPanel *get_other_panel (void);

void save_panel_dir (int idx);
const char *get_panel_dir_for (const struct WPanel * widget);

void set_hintbar (const char *str);

/* Rotating dash routines */
void use_dash (gboolean flag);  /* Disable/Enable rotate_dash routines */
void rotate_dash (void);

/*** inline functions ****************************************************************************/

#endif /* MC__LAYOUT_H */
