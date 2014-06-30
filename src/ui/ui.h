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


#ifdef __cplusplus
extern "C" {
#endif

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
#define UI_SHOW_TERM 0
#define UI_SHOW_RHIZOME_BOUNDING_BOX 0
#define UI_SHOW_RHIZOME_ORDER 0
#define UI_SHOW_STATUS 0
#define UI_SHOW_BAR 1
#define UI_ADD_BRICKS 1

#define UI_MOUSE_IDLE		 0

#define UI_LEFT_PRESSED		 1
#define UI_LEFT_CLIC		 2
#define UI_LEFT_DBCLIC		 3
#define UI_LEFT_RELEASED	 4

#define UI_RIGHT_PRESSED	 5
#define UI_RIGHT_CLIC 		 6
#define UI_RIGHT_DBCLIC		 7
#define UI_RIGHT_RELEASED	 8

#define UI_MIDDLE_PRESSED	 9
#define UI_MIDDLE_CLIC		 10
#define UI_MIDDLE_DBCLIC	 11
#define UI_MIDDLE_RELEASED	 12

#define UI_MOUSE_STATIC		13
#define UI_MOUSE_MOTION		14
#define UI_MOUSE_MOTION_PASSIVE	15

#define UI_SWAP(C, st) ((C->ui->state) = &(st)) 

#define UI_HOVER_BACKGROUND 1
#define UI_HOVER_OBJECT 2
#define UI_HOVER_BRICK 3
#define UI_HOVER_NOTHING 4


struct Context;
struct Event;
struct Clock;
struct Image;
struct Texture;
struct Screen;

typedef struct Ui
{
	int draw; 		// draw the ui
	int draw_link;
	int update_links;	// compte sets
	int bitrate;
	int do_connect;
	int do_disconnect;
	int add_bricks;
	int step;
	int step_reset;
	int fixed_menu;

	// MOUSE
	int mouse_state;
	int mouse_motion;
	int mouse_x;
	int mouse_y;
	int mouse_last_x;
	int mouse_last_y;
	int mouse_last_x_pressed;
	int mouse_last_y_pressed;
	int mouse_dx;
	int mouse_dy;
	int mouse_delta_x;
	int mouse_delta_y;
	int mouse_delta_x_last;
	int mouse_delta_y_last;
	int mouse_drag;
	int mouse_right_pressed;
	int mouse_left_pressed;

	// KEYBOARD
	int key_shift;
	int key_alt;
	int key_ctrl;

	// alphabet 
	int alphabet[160]; 

	// color
	int background_color[4];
	float front_color[4];
	float back_color[4];
	
	// font
	float font_width;
	int use_bitmap_font;

	// show
	int show_bricks;
	int show_menu;
	int show_nodes;
	int show_meshes;
	int show_objects;
	int show_intro;
	int show_intro_always;
	int show_mouse;
	int show_mouse_rec;
	int show_term;
	int show_states;
	int show_plug_state;
	int show_step;
	int show_brick_step;
	int show_sets;
	int show_rhizome_bounding_box;
	int show_rhizome_order;
	int show_status;
	int show_bar;

	// positions
	float zoom;
	float pan_x;
	float pan_y;

	float menu_pos[2];
	int mouse_size;

	// object selection flag
	int object_selection;

	// selections
	struct Node *link;
	struct Node *link_node;

	// bricks
	struct Brick *brick_selected;
	struct Brick *brick_in;
	struct Brick *brick_out;
	struct Brick *connect_brick_in;
	struct Brick *connect_brick_out;

	// screens
	struct Lst *screens;
	struct MINscreen *screen_active;
	struct Link *screen_link_active;
	int screen_direction;

	struct Camera *camera;
	struct Lst *sets;

	void (* state)( struct Context *C, struct Event *event);

	struct Clock *clock;

	float last_clic[3];
	float last_pan[3];
}t_ui;

// UI.C

void ui_switch(void);
void ui_switch_axis(void);
void ui_color_set(struct Node *node);
void ui_draw(void);
void ui_init(void);
void ui_reboot(void);

void ui_draw_rhizomes(void);
void ui_draw_intro(void);
void ui_draw_menu(void);
void ui_draw_bar(void);
void ui_draw_blocks(void);
void ui_draw_debug(void);
void ui_draw_term(void);
void ui_draw_grid(void);
void ui_draw_icon_freeze( void);

void ui_draw_start(struct Context *C);
void ui_draw_stop(struct Context *C);

t_ui *ui_new(void);
int op_ui_alphabet_switch(struct Node *node);
void op_screen_switch();
void ui_draw_sets(void);
void ui_draw_mouse(void);


void ui_event_add( int type);
void ctx_ui_old( struct Context *C);

int ui_get_window_width( struct Context *C);
int ui_get_window_height( struct Context *C);

void ui_image_draw( struct Context *C, struct Image *image, int x, int y);
void ui_texture_draw( struct Context *C, struct Texture *texture, int px, int py, int sx, int sy);

struct Lst *browser_get_bricks( void);

void screen_bricks( struct MINscreen *screen);
void screen_generic( struct MINscreen *screen);
void screen_main( struct MINscreen *screen);
void screen_intro(struct MINscreen *screen);
void screen_log(struct MINscreen *screen);

void screen_init( struct Context *C);
void screen_view3d( struct MINscreen *screen);

void screen_object( struct MINscreen *screen);
void screen_info( struct MINscreen *screen);

#ifdef __cplusplus
}
#endif

#endif
