/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

void ctx_ui_freeze(t_context *C)
{
	if(C->ui->update_links) C->ui->update_links = 0;
	else C->ui->update_links = 1;
}

void ctx_ui_switch_show_step(t_context *C)
{
	if(C->ui->show_step)
	{
		term_log("step OFF");
		C->ui->show_step = 0;
		C->ui->show_brick_step = 0;
		ctx_links_term_cleanup(C);
	}
	else 
	{
		term_log("step ON");
		C->ui->show_step = 1;
		C->ui->show_brick_step = 1;
		C->ui->step = 1;
	}
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
		if(C->app->mouse->button_right==button_pressed)	
		{
			C->event->ui.is_menu_pressed=1;
		}

		if(C->app->mouse->button_right==button_released)	
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
			C->app->keyboard->shift = 0;
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
			
			)
		{
			if(C->app->mouse->button_right==button_pressed)	
			{
				C->event->ui.is_menu_pressed=1;
			}

			if(C->app->mouse->button_right==button_released)	
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
		if(C->event->is_background_button_left_pressed)
		{
			if(C->app->mouse->button_left==button_released)
			{
				C->event->is_background_button_left_released=1;
			}
		}
		else
		{
			if(C->app->mouse->button_left==button_pressed)
			{
				C->event->is_background_button_left_pressed=1;
			}
		}
	}
}

void ctx_ui_background(t_context *C)
{
	// test background
	//if(!C->event->is_brick_transformed && !C->app->keyboard->shift)
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
			node->cls->set_state_selected(node,0);
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

// UPDATE MOUSE MENU

void ctx_block_mouse_update(t_context *C)
{
	t_node *n_menu = scene_node_get(C->scene,"block","menu_mouse");
	t_block *menu=n_menu->data;

	// show/hide menu
	ctx_ui_show_hide_menu(C);

	// update menu
	menu->cls->update(menu);
}

// DESK

void ctx_desk_update(t_context *C)
{
	t_link *l;
	t_block *b;

	for(l=C->scene->global->first;l;l=l->next)
	{
		b=l->data;
		b->cls->update(b);
	}
}

// INTRO

void ctx_ui_intro(t_context *C)
{
	t_mouse *mouse = C->app->mouse;

	if(mouse->button_left==button_pressed 
	|| mouse->button_right==button_pressed
	|| mouse->button_middle==button_pressed)
		C->ui->show_intro=0;
}

t_lst *EXE=NULL;

void ctx_exe(t_context *C)
{
	t_link *l;
	t_action *action;

	for(l=EXE->first;l;l=l->next)
	{
		action = l->data;
		action->act(action->args);
		action_free(action);
	}

	lst_cleanup(EXE);
}

void exe_init(void)
{
	EXE=lst_new("exe");
}

void exe_add_action(t_action *action)
{
	lst_add(EXE,action,"action");
}

// CTX UI 

void ctx_ui(t_context *C)
{
	// INTRO test stop 
	ctx_ui_intro(C);

	// TEST BACKGROUND 
	ctx_ui_background(C);

	// UPDATE MOUSE MENU
	ctx_block_mouse_update(C); 

	// UPDATE DESK 
	ctx_desk_update(C); 
	
	// UPDATE LINKING 
	ctx_ui_linking(C);

	// GRAPH update
	ctx_links_update(C);

	// POST EXE
	ctx_exe(C);

	// RESET 
	C->event->loop_step = 0;
	C->event->brick_delete = 0;
}
