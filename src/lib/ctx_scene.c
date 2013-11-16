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
#include "node.h"
#include "app.h"
#include "event.h"
#include "list.h"
#include "ui.h"
#include "mesh.h"
#include "object.h"
#include "ctx.h"
#include "mesh.h"
#include "object.h"
#include "brick.h"
#include "util.h"
#include "term.h"

// Selections and updates

void ctx_scene_selection(t_context *C, t_node *node, int state)
{
	t_object *object;

	if(C->event->debug_all)
	{
		if(state)
		{
			term_log("set selected %s", node_name_get(node->cls->type));
		}
		else
		{
			term_log("set diselected %s", node_name_get(node->cls->type));
		}
	}
	switch(node->cls->type)
	{
		case(nt_object):
			object = node->data;
			if(state)
			{
				if(object->is_selected)
				{
					if(object->is_edit_mode)
					{
						t_mesh *mesh = object->mesh;
						if(mesh)
						{
							int hover = mesh->state.hover_vertex;
							if( hover >= 0)
							{
								mesh->state.selected_vertex = hover;
							}
						}

					}
					else
					{
						object->is_edit_mode = 1;
						scene_switch_edit_mode(C->scene,1);
					}
				}
				else
				{
					object->is_selected = 1;
				}
			}
			else
			{
				if(C->scene->edit_mode)
				{
					scene_switch_edit_mode(C->scene,0);
				}

				object->is_selected = 0;
				object->is_edit_mode = 0;

				t_mesh *mesh = object->mesh;
				if(mesh)
				{
					mesh->state.hover_vertex = -1;
					mesh->state.selected_vertex = -1;
				}

			}
			break;
		case(nt_mesh): break;
		default:
			printf("[WARINING set selected] Unknown type %s\n", node_name_get(node->type));
			break;
	}
}

void ctx_scene_set_selected(t_context *C, t_node *node)
{
	t_node *selected=C->scene->selected;

	// disselect previous
	if(selected) 
	{
		if(!node_equal(node, selected))
		{
			ctx_scene_selection(C, selected , 0);
		}
	}

	ctx_scene_selection(C, node , 1);
	C->scene->selected=node;
	C->ui->show_nodes=0;
}

void ctx_scene_clear_selections(t_context *C)
{
	t_link *link;
	for(link=C->scene->selections->first;link;link=link->next)
	{
		t_node *node=link->data;
		ctx_scene_selection(C, node, 0);

	}
	lst_delete_all(C->scene->selections);
}

int node_hover_object(t_context *C, t_node *node)
{
	t_object *object=node->data;
	t_mesh *mesh=object->mesh;

	if(is(object->type,"point"))
	{
		if(is_mouse_over(C,object->idcol))
		{
			object->hover=1;
			return 1;
		}
		else
		{
			object->hover=0;
			return 0;
		}
	}
	else if(mesh)
	{
		if(C->scene->edit_mode)
		{
			t_node *selected = C->scene->selected;
			if(node_equal(node,selected))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(is_mouse_over(C,mesh->idcol))
			{
				object->hover=1;
				return 1;
			}
			else
			{
				object->hover=0;
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}
}

int node_hover_brick(t_context *C, t_node *node)
{
	t_brick *brick=node->data;

	if(is_mouse_over_brick(C,brick))
	{
		brick->state.is_mouse_over=1;
		return 1;
	}
	else
	{
		brick->state.is_mouse_over=0;
		return 0;
	}
}

int node_hover(t_context *C, t_node *node)
{
	switch(node->type)
	{
		case(nt_brick): return node_hover_brick(C,node); break;
		case(nt_object): return node_hover_object(C,node); break;
		default: break;
	}

	return 0;
}


void ctx_scene_update_lst(t_context *C, t_lst *lst)
{
	t_link *link;
	t_node *node;

	if(!C->ui->show_menu)
	{
		for(link=lst->first;link;link=link->next)
		{
			node = link->data;
			//if(node->cls->is_mouse_over(node))
			if(node_hover(C, node))
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
					ctx_scene_update_lst(C,C->scene->objects);
				}
			}

			//ctx_scene_update_lst(C,C->scene->objects);
		}
	}

	// cleanup
	scene_cleanup(C->scene);
}
