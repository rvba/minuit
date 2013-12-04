/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "block.h"
#include "brick.h"
#include "plug.h"
#include "list.h"

#include "sketch.h"
#include "ui.h"
#include "ctx.h"

#include "rhizome.h"
#include "set.h"
#include "app.h"
#include "op.h"

#include "event.h"
#include "graph.h"

#include "dict.h"
#include "memory.h"
#include "list.h"

void rhizome_show(t_rhizome *rhizome)
{
	printf("RHIZOME %s\n", rhizome->id.name);
	printf("BLOCKS\n");
	lst_show(rhizome->blocks);
	printf("GRAPH\n");
	graph_show(rhizome->graph);

}

/******************	DRAW	**********************/

void rhizome_draw_blocks(t_rhizome *rhizome)
{
	t_link *l;
	t_block *block;
	for(l=rhizome->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->cls->draw(block);
	}
}

// DRAW BOUNDING BOX

void rhizome_draw_bounding_box(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	if(C->ui->show_rhizome_bounding_box)
	{
		t_link *l;
		t_block *block;

		float up=0;
		float down=0;
		float left=0;
		float right=0;

		float margin = 10;

		int first_run = 1;

		if(rhizome->blocks)
		{
			for(l=rhizome->blocks->first;l;l=l->next)
			{
				block = l->data;
				float x = block->pos[0];
				float y = block->pos[1];

				if(first_run)
				{
					first_run = 0;

					left = x;
					right = x+block->width;
					up = y+block->height;
					down = y;
				}
				else
				{
					if(x <= left) left = x;
					if(x+block->width >= right) right = x+block->width;
					if(y+block->height >= up) up = y+block->height;
					if(y <= down) down = y;
				}
			}

			float p[3]={left - margin,down - margin,0};

			float w = right - left + margin*2;
			float h = up - down + margin*2;

			glLineStipple(2, 0xAAAA);
			glEnable(GL_LINE_STIPPLE);

			skt_line_rectangle(p,w,h,1,C->ui->front_color);

			glDisable(GL_LINE_STIPPLE);
		}
	}
}

// DRAW

void rhizome_draw(t_rhizome *rhizome)
{
	rhizome_draw_blocks(rhizome);
	rhizome_draw_bounding_box(rhizome);
}

/******************	EXEC	**********************/


// Get Loop Block
t_block *rhizome_block_loop_get(t_rhizome *rhizome)
{
	t_link *link;
	t_block *block;
	for(link=rhizome->blocks->first;link;link=link->next)
	{
		block = link->data;
		if(block->state.is_a_loop)
			return block;
	}
	return NULL;
}

// Exec Block Loop Only
void rhizome_exec_block_loop(t_rhizome *rhizome)
{
	// Get Loop Block
	t_block *block = rhizome_block_loop_get(rhizome);

	// Get For Brick
	t_brick *brick = block_brick_get(block,"for");

	if(brick)
	{
		t_plug *plug = &brick->plug_intern;
		plug->state.is_updated = 0;

		// Exec
		block_brick_trigger(plug);
	}
}

// EXEC

// Exec All Blocks
void rhizome_exec_blocks(t_rhizome *rhizome)
{
	t_link *link;
	t_block *block;

	for(link=rhizome->blocks->first;link;link=link->next)
	{
		block = link->data;
		block_exec(block);
	}
}

// Exec Rhizome
void rhizome_exec(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	if(C->ui->update_links)
	{
		// With LOOP
		if(rhizome->has_loop)
		{
			// Find Block Loop
			t_block *block = rhizome_block_loop_get(rhizome);

			if(block)
			{
				// Do Loop
				if(rhizome->start_loop)
				{
					C->event->loop_step++;
					// Exec Blocks
					rhizome_exec_blocks(rhizome);

					// Loop Recursive
					rhizome_exec(rhizome);
				}
				// End Loop
				else if(rhizome->end_loop)
				{
					// Reset States
					rhizome->start_loop = 0;
					rhizome->end_loop = 0;
				}
				// Start Loop
				else
				{
					// Exec Block Loop Only
					rhizome_exec_block_loop(rhizome);

					// Loop Recursive
					if(rhizome->start_loop)
						rhizome_exec(rhizome);
				}
			}
			else
			{
				printf("[ERROR garph_exec] Can't find block loop\n");
			}
		}
		// NO LOOP
		else
		{
			// Exec Blocks
			rhizome_exec_blocks(rhizome);
		}
	}
}

/******************	SETUP	**********************/

// BLOCK POS

void rhizome_set_block_pos(t_block *block, int pos)
{
	if(block->rhizome_pos < pos)
	{
		block->rhizome_pos = pos;
		t_link *link;

		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;
			if(brick->plug_out.state.is_connected)
			{
				t_plug *plug_dst = brick->plug_out.dst;
				t_brick *brick_dst = plug_dst->brick;
				t_block *block_dst = brick_dst->block;

				rhizome_set_block_pos(block_dst,pos+1);
			}
		}
	}
}

// SORT

void rhizome_sort(t_rhizome *rhizome)
{
	t_link *l;
	t_block *block;
	int pos;

	// Reset
	for(l=rhizome->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->rhizome_pos = 0;
	}

	// Set Pos
	for(l=rhizome->roots->first;l;l=l->next)
	{
		t_link *link;
		block = l->data;
		pos = 1;

		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;
			if(brick->plug_out.state.is_connected)
			{
				t_plug *plug_dst = brick->plug_out.dst;
				t_brick *brick_dst = plug_dst->brick;
				t_block *block_dst = brick_dst->block;

				rhizome_set_block_pos(block_dst,pos);
			}
		}
	}

	// Set Link order
	for(l=rhizome->blocks->first;l;l=l->next)
	{
		block=l->data;
		l->order = block->rhizome_pos;
	}
	
	// Sort List
	lst_sort_bubble(rhizome->blocks);
}

// GET ROOTS

void rhizome_get_roots(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	t_link *l;
	t_block *block;
	t_brick *brick;
	t_plug *plug_in;

	// Add Lst
	if(!rhizome->roots)
	{
		t_node *node_list = scene_add(C->scene,dt_list,"roots");
		rhizome->roots = node_list->data;
	}

	t_lst *lst = rhizome->blocks;
	t_lst *roots = rhizome->roots;

	// Cleanup Lst
	lst_cleanup(roots);

	// For Lst
	for(l=lst->first;l;l=l->next)
	{
		t_link *link;
		block = l->data;
		int add_to_roots = 1;

		for(link=block->bricks->first;link;link=link->next)
		{
			brick=link->data;
			plug_in = &brick->plug_in;

			// If Block Connected
			//if(plug_in->state.is_connected)
			if(plug_in->src)
			{
				// NOT Root
				add_to_roots = 0;
			}
		}

		if(add_to_roots)
		{
			block->state.is_root = 1;
			list_add(roots,block);
		}
		else
		{
			block->state.is_root = 0;
		}
	}
}

// SETUP

void rhizome_setup(t_rhizome *rhizome)
{
	t_link *link;
	t_block *block;
	int has_loop = 0;
	int frame_based = 0;

	// Set Rhizome Status
	for(link=rhizome->blocks->first;link;link=link->next)
	{
		block = link->data;
		if(block->state.is_a_loop) has_loop = 1;
		if(block->state.frame_based) frame_based = 1;
	}

	rhizome->has_loop = has_loop;
	rhizome->frame_based = frame_based;

	// Update Set
	set_setup(rhizome->set);

	// Set Rhizome Roots
	rhizome_get_roots(rhizome);
	rhizome_sort(rhizome);
}

// ADD

void rhizome_graph_dot_add(t_rhizome *rhizome, t_block *block)
{
	graph_dot_add(rhizome->graph, block);
}

void rhizome_graph_link_add(t_rhizome *rhizome, t_brick *brick_x, t_brick *brick_y)
{
	t_block *block_x = brick_x->block;
	t_block *block_y = brick_y->block;

	t_graph *graph = block_x->rhizome->graph;

	t_dot *dot_x = graph_dot_find(graph, block_x->id.id);
	t_dot *dot_y = graph_dot_find(graph, block_y->id.id);

	t_dash *dash = graph_dash_add(graph, dot_x, dot_y);

	dash->id_x = brick_x->id.id;
	dash->id_y = brick_y->id.id;
}


// BLOCK REMOVE

void rhizome_block_reset(t_block *block)
{
	// Reset Block State
	block->rhizome=NULL;
	block->state.is_in_rhizome = 0;

	// Add To Set
	set_block_push(block->set, block);
}

void rhizome_block_remove(t_rhizome *rhizome, t_block *block)
{
	// Reset Block State
	block->rhizome=NULL;
	block->state.is_in_rhizome = 0;

	// Add To Set
	set_block_push(block->set, block);

	// Setup
	if(rhizome) rhizome_setup(rhizome);
}

// DICT

t_symbol *rhizome_dict_symbol_get(t_dict *dict, int id)
{
	t_symbol *symbol;
	char name[_NAME_];

	snprintf(name,_NAME_,"%d",id);
	symbol = dict_pop(dict, name);

	if(!symbol)
	{
		t_lst *lst = lst_new(name);
		symbol = dict_symbol_add(dict, name, dt_lst, lst);
	}
	
	return symbol;
}

void rhizome_dict_free(t_dict *dict)
{
	t_link *link;
	t_symbol *symbol;
	t_lst *lst;

	for(link=dict->symbols->first;link;link=link->next)
	{
		symbol = link->data;
		lst = symbol->data;
		lst_cleanup(lst);
		lst_free(lst);
	}

	dict_free(dict);
}

// BUILD

void rhizome_graph_dots_build(t_rhizome *rhizome)
{
	t_link *l;
	t_lst *lst = rhizome->blocks;
	t_block *block;

	// Add Dots
	for(l=lst->first;l;l=l->next)
	{
		block = l->data;
		rhizome_graph_dot_add(rhizome, block);
	}
}

void rhizome_graph_dashs_build(t_rhizome *rhizome)
{
	t_link *link_block;
	t_link *link_brick;
	t_block *block;
	t_brick *brick;
	t_brick *brick_target;
	t_graph *graph = rhizome->graph;
	int id_x, id_y;

	// Add Dashs
	for(link_block=rhizome->blocks->first;link_block;link_block=link_block->next)
	{
		block = link_block->data;

		for(link_brick=block->bricks->first;link_brick;link_brick=link_brick->next)
		{
			brick = link_brick->data;
			id_x = brick->id.id;

			// Check Connection IN
			if(brick->plug_in.state.is_connected)
			{
				brick_target = brick->plug_in.src->brick;
				id_y = brick_target->id.id;

				if(!graph_link_exists(graph,id_x,id_y))
				{
					rhizome_graph_link_add(rhizome, brick, brick_target);
				}
			}

			// Check Connection OUT
			if(brick->plug_out.state.is_connected)
			{
				brick_target = brick->plug_out.dst->brick;
				id_y = brick_target->id.id;

				if(!graph_link_exists(graph,id_x,id_y))
				{
					rhizome_graph_link_add(rhizome, brick, brick_target);
				}
			}
		}
	}
}

// BUILD FROM DOT LIST

t_rhizome *rhizome_build_from_dot_list(t_lst *lst)
{
	t_context *C = ctx_get();
	t_link *l;
	t_dot *dot;
	t_block *block;

	// New Rhizome
	scene_store(C->scene,1);
	t_rhizome *rhizome = rhizome_add("rhizome");
	scene_store(C->scene,0);

	// Fill Rhizome
	for(l=lst->first;l;l=l->next)
	{
		dot = l->data;
		block = dot->data;
		rhizome_block_add(rhizome, block);
	}

	return rhizome;
}

// BUILD FROM LIST

t_rhizome *rhizome_build_from_list(t_lst *lst)
{
	t_context *C = ctx_get();
	t_link *l;
	t_block *block;

	// New Rhizome
	scene_store(C->scene,1);
	t_rhizome *rhizome = rhizome_add("rhizome");
	scene_store(C->scene,0);

	// Fill Rhizome
	for(l=lst->first;l;l=l->next)
	{
		block = l->data;
		rhizome_block_add(rhizome, block);
	}

	return rhizome;
}

// BUILD FROM DICT

void rhizome_build_from_dict(t_rhizome *rhizome, t_dict *dict)
{
	t_link *link;
	t_lst *lst;
	t_block *block;
	t_dot *dot;
	t_symbol *symbol;
	t_rhizome *new_rhizome;

	for(link=dict->symbols->first;link;link=link->next)
	{
		symbol = link->data;
		lst = symbol->data;

		if(lst->count > 1)
		{
			// New Rhizome
			new_rhizome = rhizome_build_from_dot_list(lst);
			rhizome_graph_dashs_build(new_rhizome);
			rhizome_setup(new_rhizome);
		}
		else
		{
			// Add To Set
			dot = lst->first->data;
			block = dot->data;
			rhizome_block_reset(block);
		}
	}
}

// SPLIT

void rhizome_graph_split(t_rhizome *rhizome, t_brick *brick_x, t_brick *brick_y)
{
	t_graph *graph = rhizome->graph;

	t_link *link;
	t_dot *dot;
	t_dict *dict = dict_make( "dict");
	t_symbol *symbol;
	t_lst *lst;

	// Remove Dash
	graph_link_remove(graph,brick_x->id.id, brick_y->id.id);

	// Disjoin Graphs
	graph_dj_set(graph);

	// Build Lists
	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;
		symbol = rhizome_dict_symbol_get(dict, dot->root);
		lst = symbol->data;
		t_id *id = (t_id *) dot->data;
		lst_add(lst, dot, id->name);
	}

	// Build Rhizome
	if(dict->count > 1)
	{
		rhizome_build_from_dict(rhizome, dict);

		// Free Old rhizome
		rhizome_delete(rhizome);
	}
	else
	{
		rhizome_setup(rhizome);
	}


	// Free
	rhizome_dict_free(dict);

}

// SWAP

void rhizome_swap(t_rhizome *src, t_rhizome *dst)
{
	t_link *l;
	t_block *block;

	for(l=src->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->rhizome = dst;
		rhizome_block_add(dst, block);
	}
}

// MERGE

void rhizome_merge(t_rhizome *src, t_rhizome *dst)
{
	t_context *C = ctx_get();
	if(src->id.id == dst->id.id)
	{
		// Update Rhizome
		rhizome_setup(src);
	}
	else
	{
		// New Rhizome
		scene_store(C->scene,1);
		t_rhizome *rhizome = rhizome_add("rhizome");
		scene_store(C->scene,0);

		// Merge Rhizomes
		rhizome_swap(src,rhizome);
		rhizome_swap(dst,rhizome);

		// Remove Old Rhizomes
		rhizome_delete(src);
		rhizome_delete(dst);

		rhizome_graph_dashs_build(rhizome);

		// Update New Rhizome
		rhizome_setup(rhizome);
	}
}

// BLOCK ADD

void rhizome_block_add(t_rhizome *rhizome, t_block *block)
{
	t_context *C = ctx_get();

	// Add to Rhizome List
	scene_store(C->scene,1);
	list_add(rhizome->blocks, block);
	block->rhizome = rhizome;
	scene_store(C->scene,0);

	// Pop from Set Blocks
	if(!block->state.is_in_rhizome) set_block_pop(block->set,block);

	// Set In Rhizome
	block->state.is_in_rhizome = 1;

	// Add To Graph
	rhizome_graph_dot_add(rhizome, block);

	// Setup Rhizome
	rhizome_setup(rhizome);
}

// DELETE

void rhizome_delete(t_rhizome *rhizome)
{
	t_context *C = ctx_get();
	t_set *set = rhizome->set;
	// Remove From Set
	list_remove_by_id(set->rhizomes, rhizome->id.id);
	// Struct Delete
	scene_struct_delete(C->scene,rhizome);
}

// ADD

t_rhizome *rhizome_add(const char *name)
{
	t_context *C = ctx_get();

	// New Rhizome
	t_node *node_rhizome = scene_add(C->scene,dt_rhizome,"rhizome");
	t_rhizome *rhizome = node_rhizome->data;

	// New Blocks Lst

	t_node *node_lst = scene_add(C->scene, dt_list, "blocks");
	t_lst *lst = node_lst->data;
	rhizome->blocks = lst;

	// New Graph
	t_graph *graph = graph_make("graph");
	rhizome->graph = graph;

	// Add To Set
	t_set *set = get_current_set(C);
	list_add(set->rhizomes,rhizome);

	rhizome->set = set;

	return rhizome;
}

// INIT

void rhizome_graph_build(t_rhizome *rhizome)
{
	t_graph *graph = graph_make("graph");
	rhizome->graph = graph;

	rhizome_graph_dots_build(rhizome);
	rhizome_graph_dashs_build(rhizome);
}

void rhizome_init(t_rhizome *rhizome)
{
	rhizome->start_loop = 0;
	rhizome->end_loop = 0;
	rhizome->graph = NULL;
}

// CLONE

t_rhizome *rhizome_clone(t_rhizome *rhizome)
{
	if(rhizome)
	{
		t_rhizome *clone = rhizome_new(rhizome->id.name);

		clone->blocks = list_clone(rhizome->blocks, dt_block);
		clone->set = NULL;

		return clone;
	}
	else
	{
		return NULL;
	}
}

// REBIND

t_rhizome *rhizome_rebind(t_scene *sc,void *ptr)
{
	t_rhizome *rhizome=(t_rhizome *)ptr;

	rebind(sc,"rhizome","blocks",(void **)&rhizome->blocks);
	rebind(sc,"rhizome","set",(void **)&rhizome->set);

	rhizome->graph = NULL;
	rhizome->roots = NULL;

	return rhizome;
}

// FREE

void _rhizome_free(t_rhizome *rhizome)
{
	if(rhizome->blocks) _list_free(rhizome->blocks, dt_block);
	mem_free( rhizome, sizeof( t_rhizome));
}

void rhizome_free(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	if(rhizome->blocks)
	{
		scene_struct_delete(C->scene,rhizome->blocks);
	}
	if(rhizome->roots)
	{
		scene_struct_delete(C->scene,rhizome->roots);
	}
	if(rhizome->graph)
	{
		graph_free(rhizome->graph);
	}
	
	mem_free( rhizome, sizeof( t_rhizome));
}

// NEW

t_rhizome *rhizome_new(const char *name)
{
	t_rhizome *rhizome = (t_rhizome *)mem_malloc(sizeof(t_rhizome));

	id_init(&rhizome->id, name);

	rhizome->blocks = NULL;
	rhizome->roots = NULL;

	rhizome->set = NULL;
	rhizome->graph = NULL;

	rhizome->has_loop = 0;
	rhizome->start_loop = 0;
	rhizome->end_loop = 0;
	rhizome->frame_based = 0;

	return rhizome;
}



