/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "op.h"
#include "context.h"
#include "scene.h"
#include "app.h"
#include "event.h"
#include "term.h"
#include "viewport.h"
#include "camera.h"
#include "ui.h"
#include "screen.h"
#include "draw.h"
#include "list.h"
#include "event.h"
#include "rhizome.h"

int switch_init=0;
int switch_frame=0;
int switch_done=0;

void ctx_switch_draw( t_context *C)
{
		if( C->draw->with_draw_pass) C->draw->with_draw_pass = 0;
		else C->draw->with_draw_pass = 1;
}

void ctx_switch_font( t_context *C)
{
	if( C->ui->use_bitmap_font)  C->ui->use_bitmap_font = 0;
	else C->ui->use_bitmap_font = 1;
}

void ctx_switch_intro(t_context *C)
{
	if(C->ui->show_intro_always) C->ui->show_intro_always = 0;
	else C->ui->show_intro_always = 1;
}

void keymap_generic( int key)
{
}

void switch_txt(t_context *C)
{
	if(C->ui->use_bitmap_font) C->ui->use_bitmap_font=0;
	else C->ui->use_bitmap_font=1;
}

void switch_sets(t_context *C)
{
	t_node *node_screen_sets = scene_node_get(C->scene,"screen","screen_sets");
	t_screen *screen_set = ( t_screen *) node_screen_sets->data;

	if(C->ui->show_sets)
	{
		C->ui->show_sets = 0;
		screen_off(screen_set);
	}
	else
	{
		C->ui->show_sets = 1;
		screen_on(screen_set);
	}
}

void show_sets(t_context *C)
{
	t_node *node_screen_sets = scene_node_get(C->scene,"screen","screen_sets");
	t_screen *screen_set = ( t_screen *) node_screen_sets->data;

	screen_set->is_active = 1;
	screen_set->is_visible = 1;
}

void switch_bricks(t_context *C)
{
	t_node *node = scene_node_get(C->scene,"screen","screen_bricks");
	t_screen *screen = ( t_screen *) node->data;

	if(C->ui->show_bricks)
	{
		C->ui->show_bricks = 0;
		screen_off(screen);
	}
	else
	{
		C->ui->show_bricks = 1;
		screen_on(screen);
	}
}

void keymap_typing( int key)
{
	t_context *C=ctx_get();
	char char_key;

	switch(key)
	{
		case RETURNKEY:
			C->event->is_typing=0; 
			break;

		case BACKSPACEKEY:
			C->event->buffer_char[C->event->buffer_char_counter-1] = '\0';
			C->event->buffer_char_counter--;
			break;
		case SHIFTKEY:
			break;

		default:
			char_key = (char)key;
			strncat(C->event->buffer_char,&char_key,1);
			C->event->buffer_char_counter++;
			break;
	}
}

void keymap_command( int key)
{
	t_context *C=ctx_get();
	t_main_event *event = C->event;
	t_camera *camera = NULL;

	if(C->scene->has_generic_viewport)
	{
		// Get default Viewport
		t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
		t_viewport *viewport = NULL;

		if(node_viewport)
		{
			viewport = ( t_viewport *) node_viewport->data;
			camera = ( t_camera *) viewport->camera;
		}
	}

	switch(key)
	{
		// plugs

		/*
		case UP_AKEY: event->switch_plug_in_flow_in = 1; break;
		case UP_QKEY: event->switch_plug_in_open_in = 1; break;
		case UP_WKEY: event->switch_plug_in_follow_in = 1; break;

		case UP_ZKEY: event->switch_plug_in_flow_out = 1; break;
		case UP_SKEY: event->switch_plug_in_open_out = 1; break;
		case UP_XKEY: event->switch_plug_in_follow_out = 1; break;

		case UP_EKEY: event->switch_plug_out_flow_in = 1; break;
		case UP_DKEY: event->switch_plug_out_open_in = 1; break;
		case UP_CKEY: event->switch_plug_out_follow_in = 1; break;

		case UP_RKEY: event->switch_plug_out_flow_out = 1; break;
		case UP_FKEY: event->switch_plug_out_open_out = 1; break;
		case UP_VKEY: event->switch_plug_out_follow_out = 1; break;
		*/

		case UP_BKEY: op_debug_all(C);break;

		case BKEY: switch_bricks(C); break;
		case EKEY: event->switch_brick_debug = 1; break;

		case STARKEY: draw_switch_axis_world(C->draw);break;

		case DOTKEY: if(camera) op_camera_reset_pos(camera);break;
		case PADFIVE: if(camera) op_camera_switch_type(camera);break;
		case PADONE: if(camera) op_camera_view_front(camera);break;
		case PADTWO: if(camera) op_camera_ortho_rotate(camera,-5,0,0);break;
		case PADTHREE: if(camera) op_camera_view_left(camera);break;
		case PADFOUR: draw_switch_scene( C->draw);break;
		case PADSEVEN: if(camera) op_camera_view_top(camera);break;
		case PADNINE: if(camera) op_camera_view_axo(camera);break;
		case PADZERO: 
				C->ui->zoom = 1;
				C->ui->pan_x = 0;
				C->ui->pan_y = 0;
				C->event->ui.pan_x = 0;
				C->event->ui.pan_y = 0;
				break;

		case QKEY: op_quit(NULL);break;
		case ZKEY: app_screen_switch_fullscreen(C->app);break;
		case XKEY: op_switch_color(C);break;
		case NKEY: C->ui->step = 1; break;
		case RKEY:
			if(C->app->keyboard->alt)
				term_reset_all();
			else
				C->ui->step_reset = 1;
			break;

		case UP_TKEY: ctx_switch_font( C); break;
		case UP_MKEY: ctx_switch_draw( C); break;

		case DKEY: C->event->brick_delete = 1; break;
		case JKEY: ctx_ui_switch_show_states(C); break;
		case FKEY: ctx_ui_freeze(C); break;
		case WKEY: ctx_switch_intro(C);

		case ESCKEY: 	

			C->draw->with_draw_pass=1; 
			C->draw->with_selection_pass=1; 
			C->draw->with_clear=1;
			C->ui->font_width=1;
			C->ui->object_selection=1;
			C->event->is_typing=0; 
			C->app->timer=0;

			break;


		case TABKEY:
			switch_sets(C);
			// reset key
			C->app->keyboard->key_pressed=0;
			break;

		case F1KEY: load_last(C);break; 
		case F2KEY: save_to_file(C);break;
		case F3KEY: save_file_increment(C);break; 
	}
}


void keymap_main( int key)
{
	t_context *C=ctx_get();
	t_main_event *event = C->event;

	if(event->is_typing)
	{
		keymap_typing(key);
	}
	else
	{
		keymap_command(key);
	}
}


void ctx_ui_keyboard_set( t_context *C, t_event *event)
{
	switch( event->type)
	{
		case SHIFTKEY:	C->ui->key_shift = 1; break;
		case ALTKEY: 	C->ui->key_alt = 1; break;
		case CTRLKEY: 	C->ui->key_ctrl= 1; ;break;
	}
}

void ctx_keyboard(t_context *C)
{
	t_screen *s = C->ui->screen_active;

	C->ui->key_shift = 0;
	C->ui->key_alt = 0;
	C->ui->key_ctrl = 0;

	if( C->app->keyboard->key_pressed)
	{
		event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, C->app->keyboard->key_pressed);
		C->app->keyboard->key_pressed=0;
	}

	if( C->app->keyboard->special)
	{
		 event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, C->app->keyboard->special);
		C->app->keyboard->special=0;
	}

	if( C->app->keyboard->modifier)
	{
		if( C->app->keyboard->shift)	 event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, SHIFTKEY);
		if( C->app->keyboard->alt)	 event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, ALTKEY);
		if( C->app->keyboard->ctrl)	 event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, CTRLKEY);
	}
}
