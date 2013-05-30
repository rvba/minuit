/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "context.h"
#include "scene.h"
#include "ctx.h"
#include "app.h"
#include "server.h"
#include "event.h"
#include "term.h"
#include "viewport.h"
#include "camera.h"
#include "ui.h"
#include "screen.h"
#include "draw.h"
#include "list.h"

int switch_init=0;
int switch_frame=0;
int switch_done=0;

void ctx_switch_intro(t_context *C)
{
	if(C->ui->always_show_intro)
		C->ui->always_show_intro = 0;
	else
		C->ui->always_show_intro = 1;
}

void keymap_generic(unsigned char key)
{
	t_context *C=ctx_get();

	switch(key)
	{
		case 'q': op_quit(NULL);break;
		case 9  : 

			op_screen_switch();
			//reset key
			C->app->keyboard->key_pressed=0;
			break;
	}
}

void switch_txt(t_context *C)
{
	if(C->ui->use_bitmap_font) C->ui->use_bitmap_font=0;
	else C->ui->use_bitmap_font=1;
}

void switch_sets(t_context *C)
{
	if(C->ui->show_sets)
	{
		C->ui->show_sets = 0;
		screen_switch_by_name("screen_main");
	}
	else
	{
		C->ui->show_sets = 1;
		screen_switch_by_name("screen_sets");
	}
}

void switch_bricks(t_context *C)
{
	t_node *node = scene_node_get(C->scene,"screen","screen_bricks");
	t_screen *screen = node->data;

	if(C->app->mouse->button_left == button_released)
	{
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
}

void keymap_typing(unsigned char key)
{
	t_context *C=ctx_get();
	t_event *event = C->event;

	char char_key;

	switch(key)
	{
		case 0:
			break;

		// RETURN
		case 13:
			event->ui.typing_end = 1;
			break;

		default:
			char_key = (char)key;
			strncat(C->event->buffer_char,&char_key,1);
			C->event->buffer_char_counter++;
			break;
	}
}

void keymap_command(unsigned char key)
{
	t_context *C=ctx_get();
	t_event *event = C->event;

	t_camera *camera = NULL;
	if(C->scene->has_generic_viewport)
	{
		// Get default Viewport
		t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
		t_viewport *viewport = NULL;

		if(node_viewport)
		{
			viewport = node_viewport->data;
			camera = viewport->camera;
		}
	}

	switch(key)
	{
		// plugs

		case 'A': event->switch_plug_in_flow_in = 1; break;
		case 'Q': event->switch_plug_in_open_in = 1; break;
		case 'W': event->switch_plug_in_follow_in = 1; break;

		case 'Z': event->switch_plug_in_flow_out = 1; break;
		case 'S': event->switch_plug_in_open_out = 1; break;
		case 'X': event->switch_plug_in_follow_out = 1; break;

		case 'E': event->switch_plug_out_flow_in = 1; break;
		case 'D': event->switch_plug_out_open_in = 1; break;
		case 'C': event->switch_plug_out_follow_in = 1; break;

		case 'R': event->switch_plug_out_flow_out = 1; break;
		case 'F': event->switch_plug_out_open_out = 1; break;
		case 'V': event->switch_plug_out_follow_out = 1; break;

		//case 'b': event->switch_brick_debug = 1; break;
		case 'b': switch_bricks(C); break;
		case 'e': event->switch_brick_debug = 1; break;

		case '*': draw_switch_axis_world(C->draw);break;

		case 'a': ctx_switch_record_video(C); break;
		case 'q': op_quit(NULL);break;
		case '.': if(camera) op_camera_reset_pos(camera);break;
		case '5': if(camera) op_camera_switch_type(camera);break;
		case '7': if(camera) op_camera_view_top(camera);break;
		case '1': if(camera) op_camera_view_front(camera);break;
		case '9': if(camera) op_camera_view_axo(camera);break;
		case '3': if(camera) op_camera_view_left(camera);break;
		case '2': if(camera) op_camera_ortho_rotate(camera,-5,0,0);break;
		case 'z': app_screen_switch_fullscreen(C->app);break;
		case 'm': C->draw->with_draw_pass=0; break;
		case 'x': op_switch_color(C);break;
		case 't': switch_txt(C);break;
		case 'l': scene_log(C->scene);break;
		case 'n': C->ui->step = 1; break;
		case 'r':
			if(C->app->keyboard->alt)
				term_reset(C->term);
			else
				C->ui->step_reset = 1;
			break;
		case 'd': C->event->brick_delete = 1; break;
		case 'k': ctx_ui_switch_show_step(C); break;
		case 'j': ctx_ui_switch_show_states(C); break;
		case 'f': ctx_ui_freeze(C); break;
		case 'w': ctx_switch_intro(C);

		case APP_KEY_ESC: 	

			C->draw->with_draw_pass=1; 
			C->draw->with_selection_pass=1; 
			C->draw->with_clear=1;
			C->ui->font_width=1;
			C->ui->object_selection=1;
			C->event->is_typing=0; 
			C->app->timer=0;

			break;

		case 127:
			printf("RESET\n");
			break;

		// tab
		case 9:
			switch_sets(C);
			// reset key
			C->app->keyboard->key_pressed=0;
			break;
		case 'c':
			server_connect(C->server,9901);
			break;
	}

	switch(C->app->keyboard->special)
	{
		case 1: load_last(C);break; 	//F1
		case 2: save_file(C);break; 	//F2
		case 3: save_file_increment(C);break; 		//F3
	}
}


void keymap_main(unsigned char key)
{
	t_context *C=ctx_get();
	t_event *event = C->event;

	if(event->ui.typing_start)
	{
		keymap_typing(key);
	}
	else
	{
		keymap_command(key);
	}
}

// trigger all active screens

void ctx_keyboard(t_context *C)
{
	t_link *l;
	t_node *n;
	t_screen *s;
	if(C->ui->screens->first)
	{
		for(l=C->ui->screens->first;l;l=l->next)
		{
			n=l->data;
			s=n->data;
			if(s->is_active)
			{
				if(!C->event->is_typing)
				{
					 s->keymap(C->app->keyboard->key_pressed);
				}
			}

		}

		// reset key
		C->app->keyboard->key_pressed=0;
		C->app->keyboard->special=0;
	}
}
