/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "event.h"
#include "list.h"
#include "ui.h"
#include "mesh.h"
#include "object.h"
#include "mesh.h"
#include "object.h"
#include "brick.h"
#include "util.h"
#include "term.h"
#include "block.h"

int ctx_show_browser;

// Selections and updates

void ctx_scene_selection(t_context *C, t_node *node, int state)
{
	t_object *object;

	if(C->event->debug_all)
	{
		if(state)
		{
			term_log("set selected %s", data_name_get(node->cls->type));
		}
		else
		{
			term_log("set diselected %s", data_name_get(node->cls->type));
		}
	}
	switch(node->cls->type)
	{
		case(dt_object):

			object = ( t_object *) node->data;
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
		case(dt_mesh): break;
		default:
			printf("[WARINING set selected] Unknown type %s\n", data_name_get(node->type));
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
		t_node *node = ( t_node *) link->data;
		ctx_scene_selection(C, node, 0);

	}
	lst_cleanup( C->scene->selections);
}

int node_hover_object(t_context *C, t_node *node)
{
	t_object *object = ( t_object *) node->data;
	t_mesh *mesh=object->mesh;

	if( object->type == dt_point)
	{
		if(COLOR_MATCH( C->event->color, object->idcol))
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
			if(COLOR_MATCH( C->event->color, mesh->idcol))
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
	t_brick *brick = ( t_brick *) node->data;

	if(ctx_mouse_hover_brick(C,brick))
	{
		brick->brick_state.is_mouse_over=1;
		brick->block->block_state.is_mouse_over = 1;
		return 1;
	}
	else
	{
		brick->brick_state.is_mouse_over=0;
		brick->block->block_state.is_mouse_over = 0;
		return 0;
	}
}

int node_hover(t_context *C, t_node *node)
{
	switch(node->type)
	{
		case(dt_brick): return node_hover_brick(C,node); break;
		case(dt_object): return node_hover_object(C,node); break;
		default: return 0; break;
	}
}

t_node *ctx_scene_hover( t_context *C, t_data_type type)
{
	t_link *l;
	t_lst *lst = NULL;
	t_node *node = NULL;
	int hover = 0;

	if( type == dt_brick && ctx_show_browser)
	{
		lst = browser_get_bricks();
		if( lst)
		{
			for(l=lst->first;l;l=l->next)
			{
				t_brick *brick = ( t_brick *) l->data;
				t_node *n  = brick->id.node;
				hover = node_hover( C, n);
				if( hover)
				{
					node = n;
					break;
				}
			}
		}

		return node;
	}
	else
	{
		switch( type)
		{
			case dt_object: lst = scene_lst_get( C->scene, dt_object); break;
			case dt_brick: lst = scene_lst_get( C->scene, dt_brick);  break;
			default: break;
		}

		if( lst)
		{
			for(l=lst->first;l;l=l->next)
			{
				node = ( t_node *) l->data;
				hover = node_hover( C, node);
				if( hover) break;
				else node = NULL;
			}
		}

		return node;
	}
}
