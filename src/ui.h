/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __UI_H
#define __UI_H


#define PRECISION "%-.2f"
#define UI_ALPHABET 0
#define UI_GL_FONT 1
#define UI_FIXED_MENU 0
#define UI_PAN_X 0
#define UI_PAN_Y 0
#define UI_ZOOM 1
#define UI_ALPHABET_COLOR 0
#define UI_DRAW 1
#define UI_SHOW_INTRO 1
#define UI_SHOW_MOUSE 0
#define UI_SHOW_TERM 1


typedef struct Ui
{
	int draw; // draw the ui
	int update_links;
	int record_camera;

	char status_top[100];

	// alphabet 
	int alphabet[160]; 

	// color
	int background_color[4];
	float front_color[4];
	float back_color[4];
	
	// font
	float font_width;
	int use_bitmap_font;

	int draw_plug_state;

	// show
	int show_menu;
	int show_nodes;
	int show_meshes;
	int show_objects;
	int show_intro;
	int show_mouse;
	int show_term;
	int show_grid;
	int show_states;
	int show_step;
	int show_brick_step;
	int show_sets;

	int step;
	int step_reset;

	int use_threading;
	int threading_on;
	int graph_updated;

	// brick flags
	int fixed_menu;
	int flow_brick;

	// positions
	float zoom;
	float pan_x;
	float pan_y;

	float menu_pos[2];
	float max[3];
	int is_max;
	int mouse_mode;

	// object selection flag
	int object_selection;

	// debug flag
	int debug;

	// selections
	struct Brick *brick_selected;
	struct Node *link;
	struct Node *link_node;

	// selected plugs

	struct Brick *brick_in;
	struct Brick *brick_out;

	// screens
	struct Lst *screens;
	struct Link *screen_link_active;
	int screen_direction;

	int mouse_size;

	struct Camera *camera;
}t_ui;

// UI.C

void ui_switch(void);
void ui_switch_axis(void);
void ui_color_set(struct Node *node);
void ui_draw(void);
void ui_init(void);
void ui_reboot(void);

void ui_draw_intro(void);
void ui_draw_lines(void);
void ui_draw_menu(void);
void ui_draw_blocks(void);
void ui_draw_status_bar(void);
void ui_draw_debug(void);
void ui_draw_term(void);
void ui_draw_grid(void);

t_ui *ui_new(void);
int op_ui_alphabet_switch(struct Node *node);
void op_screen_switch();
void ui_draw_sets(void);
void ui_draw_mouse(void);


#endif
