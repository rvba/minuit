/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "app.h"
#include "event.h"
#include "list.h"
#include "minuit.h"
#include "ui.h"

// Selections and updates

void ctx_scene_set_selected(t_context *C,void *data)
{
	t_node *selected=C->scene->selected;

	// disselect previous
	if(selected) 
	{
		selected->cls->set_state_selected(selected,0);
		
	}

	// set selected
	t_generic *g = (t_generic *)data;
	t_node *node  =scene_node_get_by_id_global(C->scene,g->id);
	node->cls->set_state_selected(node,1);
	C->scene->selected=node;
	C->ui->show_nodes=0;
}

void ctx_scene_clear_selections(t_context *C)
{
	t_link *link;
	for(link=C->scene->selections->first;link;link=link->next)
	{
		t_node *node=link->data;
		node->cls->set_state_selected(node,0);
	}
	lst_delete_all(C->scene->selections);
}


void ctx_scene_update_node(t_context *C,t_node *node)
{
	if(node->cls->is_mouse_over(node))
	{
		if(node->type==nt_light)
		{
			ctx_scene_set_selected(C,node);
		}
		else if(node->type==nt_object)
		{
			if(C->app->mouse->button_left==button_pressed)
			{
				// multi selection
				if(C->app->keyboard->shift)
				{
					if(!lst_get_node(C->scene->selections,node->id))
					{
						lst_add(C->scene->selections,node,"object");
					}
				
				}
				// single selection
				else
				{
					ctx_scene_set_selected(C,node);
				}
			}
		}
		else
		{
			//printf("[WARNING ctx_scene] Unknown type:%s\n",node->cls->type);
		}
	}
}

void ctx_scene_update_lst(t_context *C,t_lst *lst)
{
	t_link *link;
	t_node *node;

	if(!C->ui->show_menu)
	{
		for(link=lst->first;link;link=link->next)
		{
			node=link->data;
			ctx_scene_update_node(C,node);
		}
	}
}

// update selction status for meshes and lights
void ctx_scene(t_context *C)
{
	if(!C->event->is_brick_transformed)
	{
		// if object selection
		if(C->ui->object_selection)
		{
			// check for release
			if(C->event->is_selection)
			{
				if(C->app->mouse->button_left==button_released)
					C->event->is_selection=0;

			}
			// check for selection
			else
			{
				if(C->app->mouse->button_left==button_pressed)
				{
					C->event->is_selection=1;
					ctx_scene_update_lst(C,C->scene->lights);
					ctx_scene_update_lst(C,C->scene->objects);
				}
			}
		}
	}

	// cleanup
	scene_cleanup(C->scene);
}
