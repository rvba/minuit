/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "event.h"
#include "util.h"
#include "screen.h"
#include "term.h"
#include "app.h"

t_main_event *main_event_new(void)
{
	t_main_event *event = (t_main_event *) mem_malloc( sizeof( t_main_event));

	bzero(&event->ui,sizeof(t_state_event_ui));

	event->update_connections=0;

	event->state=event_idle;
	event->is_background_button_left_pressed=0;
	event->is_background_button_left_released=0;

	event->ui.is_menu_pressed=0;
	event->ui.is_menu_released=0;
	event->ui.is_menu_show=0;
	event->ui.is_menu_mouse_show=0;
	event->ui.is_menu_fixed = 0;
	event->ui.typing_start = 0;
	event->ui.typing_end = 0;
	event->ui.use_point_global_width = 1;
	event->ui.use_line_global_width = 1;
	event->ui.use_scale = 1;

	event->ui.pan = 0;
	event->ui.zoom = 0;
	event->ui.pan_x = 0;
	event->ui.pan_y = 0;

	event->is_selection=0;

	event->is_brick_transformed=0;
	event->is_brick_pressed=0;
	event->is_brick_moving=0;
	event->is_mouse_over_brick=0;
	event->is_typing=0;
	event->action=NULL;
	event->with_texture=1;
	event->with_polygon_offset=EVENT_WITH_POLYGON_OFFSET;
	event->with_clear=1;
	event->with_normal=1;
	event->with_light=1;
	event->with_depth=1;
	event->with_face=1;
	event->with_face_outline=0;
	event->with_point=0;
	event->with_edge = 0;
	event->with_edge_color = 0;
	event->with_point_id=0;
	event->with_highlight=WITH_HIGHLIGHT;
	event->sketch_with_scale=SKETCH_WITH_SCALE;
	event->with_blend=1;

	event->is_drawing=0;

	event->color[0] = 255;
	event->color[1] = 255;
	event->color[2] = 255;

	event->rec=0;
	event->rec_save=0;
	event->recording=0;
	event->show_term=CTX_SHOW_TERM;

	event->debug_select=CTX_DEBUG_SELECT;
	event->debug_mouse=CTX_DEBUG_MOUSE;
	event->debug_process=DEBUG_PROCESS;
	event->debug_console=DEBUG_CONSOLE;
	event->debug_terminal=DEBUG_TERMINAL;
	event->debug_loop = DEBUG_LOOP;
	event->debug_key = DEBUG_KEY;
	event->debug_plug = DEBUG_PLUG;
	event->debug_all = DEBUG_ALL;
	event->debug_to_file = DEBUG_TO_FILE;

	event->free_scene=FREE_SCENE;

	vset(event->color_front,1,1,1);
	vset(event->color_back,0,0,0);
	event->color_buffer=0;

	event->last_frame=0;

	bzero(&event->buffer_char[0],20);
	event->buffer_char_counter=0;
	
	event->callback=NULL;
	event->standby_int=0;
	event->standby_char='\0';
	event->standby_string=NULL;
	event->video_record = 0;
	event->video_store = 0;
	event->video_init = 0;
	event->video_frame =0;
	event->video_frame_number =0;
	event->video_stop = 0;
	event->video_stop_call = 0;
	event->video_frames_in_memory = 0;

	event->brick_delete = 0;

	event->switch_plug_in_flow_in = 0;
	event->switch_plug_in_flow_out = 0;
	event->switch_plug_in_open_in = 0;
	event->switch_plug_in_open_out = 0;
	event->switch_plug_in_follow_in = 0;
	event->switch_plug_in_follow_out = 0;

	event->switch_plug_out_flow_in = 0;
	event->switch_plug_out_flow_out = 0;
	event->switch_plug_out_open_in = 0;
	event->switch_plug_out_open_out = 0;
	event->switch_plug_out_follow_in = 0;
	event->switch_plug_out_follow_out = 0;

	event->switch_brick_debug = 0;

	event->camera_rotation = 0;
	event->brick_copying = 0;
	event->brick_cloning = 0;
	event->brick_cloning_start = 0;

	event->loop_step = 0;
	event->load_error = 0;

	event->use_threading = 1;
	event->rhizome_init = 0;

	event->color_transition = 0;
	event->color_transition_use = 1;
	event->color_transition_set = 0;
	event->color_transition_count = 0;

	event->events = lst_new( "events");
	event->events_swap = lst_new( "events_swap");

	return event;
}

void event_add( t_main_event *main_event, t_screen *screen, int x, int y, int type)
{
	t_event *event = event_new( "event");
	event->screen = screen;
	event->x = x;
	event->y = y;
	event->type = type;

	lst_add( main_event->events, event, event->id.name);
}

char *event_name( t_event *e)
{
	switch( e->type)
	{
		case AKEY: return("a"); break;
		case BKEY: return("b"); break;
		case CKEY: return("c"); break;
		case DKEY: return("d"); break;
		case EKEY: return("e"); break;
		case FKEY: return("f"); break;
		case GKEY: return("g"); break;
		case HKEY: return("h"); break;
		case IKEY: return("i"); break;
		case JKEY: return("j"); break;
		case KKEY: return("k"); break;
		case LKEY: return("l"); break;
		case MKEY: return("m"); break;
		case NKEY: return("n"); break;
		case OKEY: return("o"); break;
		case PKEY: return("p"); break;
		case QKEY: return("q"); break;
		case RKEY: return("r"); break;
		case SKEY: return("s"); break;
		case TKEY: return("t"); break;
		case UKEY: return("u"); break;
		case VKEY: return("v"); break;
		case WKEY: return("w"); break;
		case XKEY: return("x"); break;
		case YKEY: return("y"); break;
		case ZKEY: return("z"); break;

		case UP_AKEY: return("A"); break;
		case UP_BKEY: return("B"); break;
		case UP_CKEY: return("C"); break;
		case UP_DKEY: return("D"); break;
		case UP_EKEY: return("E"); break;
		case UP_FKEY: return("F"); break;
		case UP_GKEY: return("G"); break;
		case UP_HKEY: return("H"); break;
		case UP_IKEY: return("I"); break;
		case UP_JKEY: return("J"); break;
		case UP_KKEY: return("K"); break;
		case UP_LKEY: return("L"); break;
		case UP_MKEY: return("M"); break;
		case UP_NKEY: return("N"); break;
		case UP_OKEY: return("O"); break;
		case UP_PKEY: return("P"); break;
		case UP_QKEY: return("Q"); break;
		case UP_RKEY: return("R"); break;
		case UP_SKEY: return("S"); break;
		case UP_TKEY: return("T"); break;
		case UP_UKEY: return("U"); break;
		case UP_VKEY: return("V"); break;
		case UP_WKEY: return("W"); break;
		case UP_XKEY: return("X"); break;
		case UP_YKEY: return("Y"); break;
		case UP_ZKEY: return("Z"); break;

		case STARKEY: return("*"); break;
		case DOTKEY: return("."); break;
		case F1KEY: return("F1"); break;
		case F2KEY: return("F2"); break;
		case F3KEY: return("F3"); break;

		case TABKEY: return("TAB"); break;
		case RETURNKEY: return("RETURN"); break;
		case BACKSPACEKEY: return("BACKSPACE"); break;

		case PADZERO: return("PAD 0"); break;
		case PADONE: return("PAD 1"); break;
		case PADTWO: return("PAD 2"); break;
		case PADTHREE: return("PAD 3"); break;
		case PADFOUR: return("PAD 4"); break;
		case PADFIVE: return("PAD 5"); break;
		case PADSIX: return("PAD 6"); break;
		case PADSEVEN: return("PAD 7"); break;
		case PADEIGHT: return("PAD 8"); break;
		case PADNINE: return("PAD 9"); break;
		case MOUSE_STATIC: 		return("MOUSE STATIC"); break;
		case MOUSE_MOTION_PASSIVE: 	return("MOUSE MOTION PASSIVE"); break;
		case MOUSE_MOTION: 		return("MOUSE MOTION"); break;
		case MOUSE_LEFT_PRESSED: 	return("MOUSE BUTTON LEFT PRESSED"); break;
		case MOUSE_LEFT_RELEASED: 	return("MOUSE BUTTON LEFT RELEASED"); break;
		case MOUSE_RIGHT_PRESSED: 	return("MOUSE BUTTON RIGHT PRESSED"); break;
		case MOUSE_RIGHT_RELEASED: 	return("MOUSE BUTTON RIGHT RELEASED"); break;
		case MOUSE_MIDDLE_PRESSED: 	return("MOUSE BUTTON MIDDLE PRESSED"); break;
		case MOUSE_MIDDLE_RELEASED: 	return("MOUSE BUTTON MIDDLE RELEASED"); break;

		case MOUSE_LEFT_LONG: return("MOUSE_LEFT_LONG"); break;

		case SHIFTKEY: return("SHIFTKEY"); break;
		case ALTKEY: return("ALTKEY"); break;
		case CTRLKEY: return("CTRLKEY"); break;

		case UI_BRICK_RELEASED: return("UI_BRICK_RELEASED"); break;
		case UI_BLOCK_RELEASED: return("UI_BLOCK_RELEASED"); break;
		case UI_BLOCK_MOVE: return("UI_BLOCK_MOVE"); break;

		default: return("?");
	}
}

void event_show( t_event *event)
{
	printf("%s %d %d\n", event_name( event), event->x, event->y);
}

void event_free( t_event *event)
{
	mem_free( event, sizeof( t_event));
}

void event_log( t_event *e)
{
	char msg[128];

	t_context *C = ctx_get();
	t_term *term = term_get( "term_event");

	char *name = event_name( e);
	snprintf( msg, 128,"%d %s", C->app->frame, name);

	if( term)
	{
		term_print( term, msg);
	}
}

t_event *event_new( const char *name)
{
	t_event *event = mem_malloc( sizeof( t_event));
	id_init( &event->id, name);

	event->screen = NULL;
	event->x = 0;
	event->y = 0;

	return event;
}
