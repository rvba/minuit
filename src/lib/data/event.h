/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __EVENT_H
#define __EVENT_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_WITH_POLYGON_OFFSET 0
#define SKETCH_WITH_SCALE 1

#define CTX_SHOW_TERM 1
#define CTX_FIXED_BUTTON 0
#define CTX_DEBUG_MOUSE 0
#define CTX_DEBUG_SELECT 0

#define WITH_HIGHLIGHT 0
#define FREE_SCENE 0

struct MINscreen;

#define AKEY		'a' 
#define BKEY		'b'
#define CKEY		'c'
#define DKEY		'd'
#define EKEY		'e'
#define FKEY		'f'
#define GKEY		'g'
#define HKEY		'h'
#define IKEY		'i'
#define JKEY		'j'
#define KKEY		'k'
#define LKEY		'l'
#define MKEY		'm'
#define NKEY		'n'
#define OKEY		'o'
#define PKEY		'p'
#define QKEY		'q'
#define RKEY		'r'
#define SKEY		's'
#define TKEY		't'
#define UKEY		'u'
#define VKEY		'v'
#define WKEY		'w'
#define XKEY		'x'
#define YKEY		'y'
#define ZKEY		'z'

#define ZEROKEY		'0'
#define ONEKEY		'1'
#define TWOKEY		'2'
#define THREEKEY	'3'
#define FOURKEY		'4'
#define FIVEKEY		'5'
#define SIXKEY		'6'
#define SEVENKEY	'7'
#define EIGHTKEY	'8'
#define NINEKEY		'9'

#define UP_AKEY		'A'
#define UP_BKEY		'B'
#define UP_CKEY		'C'
#define UP_DKEY		'D'
#define UP_EKEY		'E'
#define UP_FKEY		'F'
#define UP_GKEY		'G'
#define UP_HKEY		'H'
#define UP_IKEY		'I'
#define UP_JKEY		'J'
#define UP_KKEY		'K'
#define UP_LKEY		'L'
#define UP_MKEY		'M'
#define UP_NKEY		'N'
#define UP_OKEY		'O'
#define UP_PKEY		'P'
#define UP_QKEY		'Q'
#define UP_RKEY		'R'
#define UP_SKEY		'S'
#define UP_TKEY		'T'
#define UP_UKEY		'U'
#define UP_VKEY		'V'
#define UP_WKEY		'W'
#define UP_XKEY		'X'
#define UP_YKEY		'Y'
#define UP_ZKEY		'Z'

#define STARKEY		'*'
#define DOTKEY		'.'

#define ESCKEY		27
#define TABKEY		9

#define	F1KEY 		1
#define	F2KEY 		2
#define	F3KEY 		3

#define RETURNKEY 	13
#define BACKSPACEKEY	8

#define PADZERO		48
#define PADONE		49
#define PADTWO		50
#define PADTHREE	51
#define PADFOUR		52
#define PADFIVE		53
#define PADSIX		54
#define PADSEVEN	55
#define PADEIGHT	56
#define PADNINE		57

#define SHIFTKEY	200
#define ALTKEY		201
#define CTRLKEY		202

#define UP_KEY 203
#define DOWN_KEY 204
#define LEFT_KEY 205
#define RIGHT_KEY 206

#define KEYBOARD_STOP	300

#define MOUSE_START 700

#define MOUSE_STATIC 800
#define MOUSE_MOTION_PASSIVE 801
#define MOUSE_MOTION 802
#define MOUSE_LEFT_PRESSED 803
#define MOUSE_LEFT_RELEASED 840
#define MOUSE_RIGHT_PRESSED 841
#define MOUSE_RIGHT_RELEASED 842
#define MOUSE_MIDDLE_PRESSED 843
#define MOUSE_MIDDLE_RELEASED 844

#define MOUSE_LEFT_LONG		845
#define MOUSE_WHEEL_UP		846
#define MOUSE_WHEEL_DOWN	847
#define MOUSE_END 850


#define EVENT_KEYBOARD( event) ( event >= 0 && event <= KEYBOARD_STOP)
#define EVENT_MOUSE( event) ( event >= MOUSE_START && event <= MOUSE_END)
#define EVENT_MOUSE_WHEEL( event) ( ((event) == MOUSE_WHEEL_UP) || ((event) == MOUSE_WHEEL_DOWN))

#define UI_BRICK_RELEASED 1100
#define UI_BLOCK_MOVE 		1101
#define UI_BLOCK_RELEASED 	1200
#define BLOCK_DELETE		 	1201
#define UI_SELECT_RESET		1202
#define UI_BLOCK_SWAP		1203

#define BRICK_NULL 		0
#define BRICK_LEFT 		1
#define BRICK_RIGHT 		2
#define BRICK_PLUG_IN 		3
#define BRICK_PLUG_OUT 		4

#define UI_EVENT_NULL  0
#define UI_EVENT_BROWSER_SHOW 1001

typedef struct State_Event_UI
{
	int start_x:1;
	int start_y:1;
	int end_x:1;
	int end_y:1;

	int pan:1;
	int zoom:1;
	
	int is_selection:1;

	int use_point_global_width:1;
	int use_line_global_width:1;
	int use_scale:1;

	int pan_x;
	int pan_y;

}t_state_event_ui;

typedef struct Main_Event
{
	t_state_event_ui ui;

	int update_connections;

	// color
	int color[4];
	float color_front[3];
	float color_back[3];
	int color_buffer;

	// mouse
	int is_mouse_left_pressed;
	int is_mouse_left_released;
	int is_mouse_over_brick;
	int start_x;
	int start_y;
	int end_x;
	int end_y;

	int is_selection;

	int is_typing;

	char *action;

	// draw 
	int with_texture;
	int with_polygon_offset;
	int with_normal;
	int with_light;
	int with_clear;
	int with_depth;
	int with_point;
	int with_point_id;
	int with_edge;
	int with_edge_color;
	int with_face;
	int with_face_outline;
	int with_highlight;
	int with_blend;

	// sketch
	int sketch_with_scale;

	int rec;
	int recording;
	int rec_save;
	int frame_rec;
	int show_term;

	int debug_select;
	int debug_mouse;
	int debug_process;
	int debug_console;
	int debug_terminal;
	int debug_loop;
	int debug_key;
	int debug_plug;
	int debug_all;
	int debug_to_file;

	int free_scene;
	int last_frame; //last recorded camera

	char buffer_char[ _NAME_LONG_];
	int buffer_char_counter;

	void (* callback)(void);

	int video_record;
	int video_store;
	int video_stop_call;
	int video_stop;
	int video_init;
	int video_frame;
	int video_frame_number;
	int video_frames_in_memory;

	int brick_delete;

	int switch_brick_debug;

	int loop_step;
	int load_error;

	int use_threading;
	int rhizome_init;

	int color_transition;
	int color_transition_use;
	int color_transition_set;
	int color_transition_count;

	struct Lst *events;
	struct Lst *events_swap;

}t_main_event;

typedef struct Event
{
	t_id id;
	struct MINscreen *screen;
	int type;
	int x;
	int y;

}t_event;

void 		event_show( t_event *event);
void 		event_free( t_event *event);
void		event_add( t_main_event *main_event, struct MINscreen *screen, int x, int y, int type);
void 		event_dispach( t_main_event *main_event);
void *		event_new( const char *name);
t_main_event *	main_event_new(void);
void 		event_log( t_event *e);
void 		event_ui_log( int type);
char *		event_name( int type);

#ifdef __cplusplus
}
#endif

#endif
