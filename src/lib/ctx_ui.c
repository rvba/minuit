/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

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

	for(link = C->scene->sets->first; link; link = link->next)
	{
		t_node *node = link->data;
		t_set *set = node->data;
		t_lst *lst = set->lst;
		t_link *l = lst->first;

		if(l)
		{
			for(;l;l=l->next)
			{
				t_block *b;
				b=l->data;
				b->cls->update(b);
			}
		}
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

// EXE

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

// THREADING

void *ctx_ui_process(void *ptr)
{
	t_context *C = ctx_get();
	t_process *process = (t_process *) ptr;

	process->busy = 1;

	if(C->ui->use_threading)
	{
		ctx_links_update(C);
	}

	if(C->ui->graph_updated)
		process->busy = 0;

	return NULL;
}

void ctx_ui_threading(t_context *C)
{
	if(C->ui->use_threading)
	{
		// Start Threading
		if(!C->ui->threading_on)
		{
			t_process *process = process_add(C,"ui",ctx_ui_process);
			process_launch(process);

			C->ui->threading_on = 1;
		}
	}
	else
	{
		// Stop Threading
		if(C->ui->threading_on)
		{
			t_process *process = engine_process_get(C->engine, "ui");

			if(!process->busy)
			{
				engine_process_remove(C->engine,"ui");

				C->ui->threading_on = 0;
			}
		}
	}
}

// CTX UI 

void ctx_ui(t_context *C)
{
	// Threading
	ctx_ui_threading(C);

	// intro test stop 
	ctx_ui_intro(C);

	// test background 
	ctx_ui_background(C);

	// update blocks
	ctx_block_mouse_update(C); 
	ctx_block_screen_update(C);
	ctx_block_set_update(C);
	
	// update linking 
	ctx_ui_linking(C);

	// graph update
	if(!C->ui->use_threading && C->ui->graph_updated)
	ctx_links_update(C);

	// post exe
	ctx_exe(C);

	// reset 
	C->event->loop_step = 0;
	C->event->brick_delete = 0;
}
