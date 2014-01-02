/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "context.h"
#include "scene.h"
#include "action.h"
#include "ctx.h"
#include "app.h"
#include "process.h"
#include "engine.h"
#include "event.h"
#include "term.h"
#include "draw.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "list.h"
#include "data.h"
#include "plug.h"
#include "set.h"
#include "screen.h"
#include "rhizome.h"
#include "op.h"

void ctx_ui_freeze(t_context *C)
{
	if(C->ui->update_links) C->ui->update_links = 0;
	else C->ui->update_links = 1;
}

void ctx_ui_switch_show_step(t_context *C)
{
}

void ctx_ui_switch_show_states(t_context *C)
{
	if(C->ui->show_states)
		C->ui->show_states = 0;
	else
		C->ui->show_states = 1;
}

void ctx_ui_menu_hide(t_context *C)
{
	int release = 1;

	if(C->event->ui.is_menu_fixed)
	{
		if( C->ui->mouse_state == UI_RIGHT_PRESSED)
		{
			C->event->ui.is_menu_pressed=1;
		}

		if( C->ui->mouse_state == UI_RIGHT_RELEASED)
		{
			if(C->event->ui.is_menu_pressed)
				C->event->ui.is_menu_released=1;
		}
		if(C->event->ui.is_menu_pressed && C->event->ui.is_menu_released)
		{
			C->event->ui.is_menu_pressed = 0;
			C->event->ui.is_menu_released = 0;
			release = 1;
			C->event->ui.is_menu_fixed = 0;
		}
		else
		{
			release = 0;
		}

	}
	else
	{
		if(C->app->keyboard->shift)
		{
			C->event->ui.is_menu_fixed = 1;
			//C->app->keyboard->shift = 0;
			release = 0;
		}
	}

	if(release)
	{
		C->ui->show_menu=0;
		C->event->ui.is_menu_show=0;
		C->event->ui.is_menu_mouse_show=0;
		C->event->ui.is_menu_pressed=0;
		C->event->ui.is_menu_released=0;
	}
}

// MENU SHOW

void ctx_ui_menu_show(t_context *C)
{
	C->ui->show_menu=1;

	C->event->ui.is_menu_show=1;
	C->event->ui.is_menu_mouse_show=1;
	C->event->ui.is_menu_pressed=0;
	C->event->ui.is_menu_released=0;

	// set new menu location  

	C->ui->menu_pos[0] = C->app->mouse->x + 10;
	C->ui->menu_pos[1] = C->app->mouse->y + 10;

	t_node *menu = scene_node_get(C->scene,"block","menu_mouse");

	t_block *m = menu->data;
	m->pos[0] = C->ui->menu_pos[0];
	m->pos[1] = C->ui->menu_pos[1];
}

// MENU LOGIC

void ctx_ui_show_hide_menu(t_context *C)
{
	if(!C->event->is_brick_transformed)
	{
		// check right brick status
		if(
			!C->event->camera_rotation
			&& !C->event->ui.pan
			&& !C->event->ui.zoom
			&& !C->app->keyboard->alt
			&& !C->app->keyboard->ctrl
			
			)
		{
			// Set Pressed
			if( C->ui->mouse_state == UI_RIGHT_PRESSED)	
			{
				C->event->ui.is_menu_pressed=1;
			}

			// Set Released
			if( C->ui->mouse_state == UI_RIGHT_RELEASED)	
			{
				if(C->event->ui.is_menu_pressed)
					C->event->ui.is_menu_released=1;
			}
		}

		// show/hide menu
		if(C->event->ui.is_menu_show)
		{
			if(C->event->ui.is_menu_pressed && C->event->ui.is_menu_released)
			{
				ctx_ui_menu_hide(C);
			}
		}
		else
		{
			if(C->event->ui.is_menu_pressed && C->event->ui.is_menu_released)
			{
				ctx_ui_menu_show(C);
			}
		}
	}
}

// BACKGROUND

void ctx_ui_background_button_left_test(t_context *C)
{
	if(is_mouse_over_background(C))
	{
		if( C->ui->mouse_state == UI_LEFT_PRESSED)
		{
			C->event->is_background_button_left_pressed=1;
		}
		else
		{
			if( C->ui->mouse_state == UI_LEFT_RELEASED)
			{
				C->event->is_background_button_left_released=1;
			}
		}
	}
}

void ctx_ui_background(t_context *C)
{
	// test background
	if(!C->event->is_brick_transformed)
	{
		ctx_ui_background_button_left_test(C);
	}

	// if background clic+release
	if(
		   C->event->is_background_button_left_pressed
		&& C->event->is_background_button_left_released)
	{
		t_node *node=C->scene->selected; 

		// desselect current node
		if (node)
		{
			ctx_scene_selection(C, node, 0);
			C->scene->selected=NULL; 
		}

		// desselect other selected nodes
		ctx_scene_clear_selections(C);

		// hide menu
		ctx_ui_menu_hide(C);

		// reset background state
		C->event->is_background_button_left_pressed=0;
		C->event->is_background_button_left_released=0;
	}
}

// LINKING

void ctx_ui_linking(t_context *C)
{
	// check last plug_in 
	t_brick *brick_in=C->ui->brick_in;

	if(brick_in)
	{
		t_plug *plug_in=&brick_in->plug_in;
		
		// remove plug_in if node blured
		if(C->event->state==event_linking)
		{
			if(plug_in)
			{
				if(!is_mouse_over_plug(C,plug_in))
				{
					C->ui->brick_in=NULL;
				}
			}
		}
	}

	// mouse pos

	C->event->end_x=(C->app->mouse->x-C->ui->pan_x)/C->ui->zoom;
	C->event->end_y=(C->app->mouse->y-C->ui->pan_y)/C->ui->zoom;
}

// SCREENS

void ctx_block_screen_update(t_context *C)
{
	t_link *l=C->ui->screens->first;
	t_node *n;
	t_screen *s;
	t_block *block;

	if(l)
	{
		for(;l;l=l->next)
		{
			n=l->data;
			s=n->data;
			if(s->is_visible) //active
			{
				if(s->blocks)
				{
					t_link *link;
					for(link=s->blocks->first;link;link=link->next)
					{
						block = link->data;
						cls_block_generic_update(block);
					}
				}
			}
		}
	}
}

// MOUSE 

void ctx_block_mouse_update(t_context *C)
{
	t_node *n_menu = scene_node_get(C->scene,"block","menu_mouse");
	t_block *menu=n_menu->data;

	// show/hide menu
	ctx_ui_show_hide_menu(C);

	// update menu
	menu->cls->update(menu);
}

// SETS

void ctx_block_set_update(t_context *C)
{
	t_link *link;

	// For All Sets
	for(link = C->scene->sets->first; link; link = link->next)
	{
		t_node *node = link->data;
		t_set *set = node->data;
		t_lst *lst = set->blocks;
		t_link *l = lst->first;

		//if(!set->processing)
		//{
			if(l)
			{
				for(;l;l=l->next)
				{
					t_block *b;
					b=l->data;
					// Block Update
					b->cls->update(b);
				}
			}

			l = set->rhizomes->first;

			for(;l;l=l->next)
			{
				t_rhizome *rhizome = l->data;
				t_block *block;  
				t_link *l_block = rhizome->blocks->first;

				for(;l_block;l_block = l_block->next)
				{
					block = l_block->data;
					// Block Update
					block->cls->update(block);
				}
			}
		//}
	}
}

// INTRO

void ctx_ui_intro(t_context *C)
{
	if(
		( C->ui->mouse_state == UI_LEFT_PRESSED)
		|| ( C->ui->mouse_state == UI_RIGHT_PRESSED)
		|| ( C->ui->mouse_state == UI_MIDDLE_PRESSED)
		)
		C->ui->show_intro=0;
}

// EXE

t_lst *EXE=NULL;

int action_check( t_action *action)
{
	t_brick *brick = action->brick;
	t_set *set = brick->block->set;
	if(set->processing) return 0;
	else return 1;
}

void ctx_exe(t_context *C)
{
	t_link *l;
	t_action *action;

	t_lst *tmp = lst_copy( EXE);
	lst_cleanup(EXE);

	for(l=tmp->first;l;l=l->next)
	{
		action = l->data;
		if( action_check( action))
		{
			action->act(action);
			action_free(action);
		}
		else
		{
			exe_add_action( action);
		}
	}

	lst_free( tmp);
}

void exe_init(void)
{
	EXE=lst_new("exe");
}

void exe_add_action(t_action *action)
{
	lst_add(EXE,action,"action");
}

void ctx_do_connections(t_context *C)
{
	if(C->ui->do_connect)
	{
		_cls_brick_connect(C->ui->connect_brick_in,C->ui->connect_brick_out);
		C->ui->do_connect = 0;
		C->ui->connect_brick_in = NULL;
		C->ui->connect_brick_out = NULL;
	}

	if(C->ui->do_disconnect)
	{
		_cls_brick_disconnect(C->ui->connect_brick_in);
		C->ui->do_disconnect = 0;
		C->ui->connect_brick_in = NULL;
	}
}

//

void ctx_ui_state_default( t_context *C, t_event *e);
void ctx_ui_motion( t_context *C, t_event *e);

void ctx_ui_state_hover_object( t_context *C, t_event *e)
{
	switch( e->type)
	{
	}
}

void ctx_ui_state_hover_brick( t_context *C, t_event *e)
{
}

void ctx_ui_state_intro( t_context *C, t_event *e)
{
	if( e->type == MOUSE_LEFT_PRESSED || e->type == MOUSE_RIGHT_PRESSED)
	{
		C->ui->show_intro=0;
		UI_TRANS( C, ctx_ui_state_default); 
	}
}

void ctx_ui_state_menu( t_context *C, t_event *e)
{
	t_node *node = scene_node_get( C->scene, "block", "menu_mouse");
	t_block *block = node->data;

	if( !C->ui->show_menu)
	{
		block->pos[0] = C->app->mouse->x + 10;
		block->pos[1] = C->app->mouse->y + 10;
	
		C->ui->show_menu = 1; 
	}

	block->cls->dispatch( block);

	switch( e->type)
	{
		case MOUSE_RIGHT_PRESSED:
			C->ui->show_menu = 0;
			block_unstore( block);
			UI_TRANS( C, ctx_ui_state_default);
			break;
	}
}

void ctx_ui_state_hover_background( t_context *C, t_event *e)
{
	switch( e->type)
	{
		case MOUSE_RIGHT_PRESSED: 
			UI_TRANS( C, ctx_ui_state_menu); 
			break;

		case MOUSE_MOTION: ctx_ui_motion( C, e); break;
	}
}

int ctx_ui_hover_background( t_context *C)
{
	return is_mouse_over_background( C);
}

int ctx_ui_hover_object( t_context *C)
{
	return 0;
}

int ctx_ui_hover_brick( t_context *C)
{
	return 0;
}

int ctx_ui_state_hover( t_context *C)
{
	if	( ctx_ui_hover_background( C))	 	return UI_HOVER_BACKGROUND;
	else if	( ctx_ui_hover_object( C)) 		return UI_HOVER_OBJECT;
	else if	( ctx_ui_hover_brick( C)) 		return UI_HOVER_BRICK;
	else 						return UI_HOVER_NOTHING;
}

void ctx_ui_motion( t_context *C, t_event *e)
{
}

void ctx_ui_state_default( t_context *C, t_event *e)
{
	switch( e->type)
	{
		case MOUSE_RIGHT_PRESSED: 
			UI_TRANS( C, ctx_ui_state_menu);
			break;

		default: break;
	}
}

void ctx_ui_dispatch( t_context *C)
{
	t_link *l;
	t_event *e;
	for(l=C->event->events->first;l;l=l->next)
	{
		e = l->data;
		C->ui->state( C, e);
	}
}

void ctx_ui_hover_reset( t_context *C)
{
	t_node *node = NULL;
	if( C->scene->hover)
	{
		node = C->scene->hover;
		if( node->cls->type == dt_brick)
		{
			t_brick *brick = node->data;
			brick->state.is_mouse_over = 0;
		}
	}
}

void ctx_ui_hover( t_context *C)
{
	ctx_ui_hover_reset( C);

	if( ctx_ui_hover_background( C))
	{
		C->scene->hover = NULL;
		C->scene->hover_type = dt_null;
	}
	else
	{
		t_node *node = NULL;
		node = ctx_scene_hover( C, dt_object);
		if( node)
		{
			C->scene->hover = node;
			C->scene->hover_type = dt_object;
		}
		else
		{
			node = ctx_scene_hover( C, dt_brick);
			if( node)
			{
				C->scene->hover = node;
				C->scene->hover_type = dt_brick;
			}
			else
			{
				C->scene->hover = NULL;
				C->scene->hover_type = dt_null;
			}
		}
	}
}


/* ****************************************************** */

void ctx_ui_mouse_motion( t_context *C, int x, int y)
{
	t_ui *ui = C->ui;
	ui->mouse_x = x;
	ui->mouse_y = y;

	int dx;
	int dy;

	int last_x = ui->mouse_last_x;
	int last_y = ui->mouse_last_y;

	dx = x - last_x;
	dy = y - last_y;

	ui->mouse_dx = dx;
	ui->mouse_dy = dy;

	ui->mouse_last_x = x;
	ui->mouse_last_y = y;

}

inline int mouse_pressed( int state)
{
	if(
		state == UI_LEFT_PRESSED ||
		state == UI_RIGHT_PRESSED ||
		state == UI_MIDDLE_PRESSED)
		return 1;
	else
		return 0;
}

void ctx_ui_mouse_delta( t_ui *ui)
{
	if( mouse_pressed( ui->mouse_state))
	{
		if( ui->mouse_drag)
		{
			ui->mouse_delta_x = ui->mouse_x - ui->mouse_last_x_pressed;
			ui->mouse_delta_y = ui->mouse_y - ui->mouse_last_y_pressed;
		}
		else
		{
			ui->mouse_last_x_pressed = ui->mouse_x;
			ui->mouse_last_y_pressed = ui->mouse_y;
		}
	}
	else
	{
		ui->mouse_delta_x = 0;
		ui->mouse_delta_y = 0;
		ui->mouse_last_x_pressed = 0;
		ui->mouse_last_y_pressed = 0;
	}
}


void ctx_ui_mouse_set( t_context *C, t_event *event)
{
	int type = event->type;
	C->ui->mouse_motion = UI_MOUSE_STATIC;

	switch( type)
	{
		case MOUSE_LEFT_PRESSED: C->ui->mouse_state = UI_LEFT_PRESSED; break;
		case MOUSE_LEFT_RELEASED: C->ui->mouse_state = UI_LEFT_RELEASED; break;

		case MOUSE_RIGHT_PRESSED: C->ui->mouse_state = UI_RIGHT_PRESSED; break;
		case MOUSE_RIGHT_RELEASED: C->ui->mouse_state = UI_RIGHT_RELEASED; break;

		case MOUSE_MIDDLE_PRESSED: C->ui->mouse_state = UI_MIDDLE_PRESSED; break;
		case MOUSE_MIDDLE_RELEASED: C->ui->mouse_state = UI_MIDDLE_RELEASED; break;
	}

	if( type == MOUSE_MOTION)		C->ui->mouse_motion = UI_MOUSE_MOTION;
	else if( type == MOUSE_MOTION_PASSIVE)	C->ui->mouse_motion = UI_MOUSE_MOTION_PASSIVE;
	
	if( mouse_pressed( C->ui->mouse_state) && C->ui->mouse_motion == UI_MOUSE_MOTION) C->ui->mouse_drag = 1;
	else C->ui->mouse_drag = 0;

	ctx_ui_mouse_delta( C->ui);
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

void ctx_ui(t_context *C)
{
	if(1)
	{
	if(C->event->color_transition_use && C->event->color_transition)
	{
		op_set_color(C,2);
	}
	// intro test stop 
	ctx_ui_intro(C);

	// test background 
	ctx_ui_background(C);

	if( C->ui->mouse_state == UI_LEFT_PRESSED) C->event->ui.typing_end = 1;
	C->event->is_mouse_over_brick = 0;

	// update blocks
	ctx_block_mouse_update(C); 
	ctx_block_screen_update(C);
	ctx_block_set_update(C);

	// update connections
	ctx_do_connections(C);
	
	// update linking 
	ctx_ui_linking(C);

	// post exe
	ctx_exe(C);

	// reset 
	C->event->loop_step = 0;
	C->event->brick_delete = 0;
	}
	else
	{
		ctx_ui_hover( C);
		ctx_ui_dispatch( C);
	}
}

void ctx_ui_init( t_context *C)
{
	C->ui->state = ctx_ui_state_intro;
}
