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

#include "graph.h"
#include "set.h"
#include "app.h"
#include "op.h"

#include "event.h"

void graph_setup(t_graph *graph)
{
	t_link *link;
	t_block *block;
	int has_loop = 0;
	int frame_based = 0;

	for(link=graph->blocks->first;link;link=link->next)
	{
		block = link->data;
		if(block->state.is_a_loop) has_loop = 1;
		if(block->state.frame_based) frame_based = 1;
	}

	graph->has_loop = has_loop;
	graph->frame_based = frame_based;

	// Update Set
	set_setup(graph->set);
}

void graph_draw(t_graph *graph)
{
	graph_draw_blocks(graph);
	graph_draw_bounding_box(graph);
	graph_get_roots(graph);
	graph_sort(graph);
}

void graph_delete(t_graph *graph)
{
	t_context *C = ctx_get();
	t_set *set = graph->set;
	// Remove From Set
	list_remove_by_id(set->graphs,graph->id);
	// Struct Delete
	scene_struct_delete(C->scene,graph);
}

// LOOP

// Get Loop Block
t_block *graph_block_loop_get(t_graph *graph)
{
	t_link *link;
	t_block *block;
	for(link=graph->blocks->first;link;link=link->next)
	{
		block = link->data;
		if(block->state.is_a_loop)
			return block;
	}
	return NULL;
}

// Exec Block Loop Only
void graph_exec_block_loop(t_graph *graph)
{
	// Get Loop Block
	t_block *block = graph_block_loop_get(graph);

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
void graph_exec_blocks(t_graph *graph)
{
	t_link *link;
	t_block *block;

	for(link=graph->blocks->first;link;link=link->next)
	{
		block = link->data;
		block_exec(block);
	}
}

// Exec Graph
void graph_exec(t_graph *graph)
{
	t_context *C = ctx_get();

	// Setup
	graph_setup(graph);

	if(graph->has_loop)
	{
		// Find Block Loop
		t_block *block = graph_block_loop_get(graph);

		if(block)
		{
			// Do Loop
			if(graph->start_loop)
			{
				C->event->loop_step++;
				// Exec Blocks
				graph_exec_blocks(graph);

				// Loop Recursive
				graph_exec(graph);
			}
			// End Loop
			else if(graph->end_loop)
			{
				// Reset States
				graph->start_loop = 0;
				graph->end_loop = 0;
				graph->done = 1;
			}
			// Start Loop
			else
			{
				// Loop
				if(graph->done)
				{
					// Exit
					graph->done=0;
				}
				else
				{
					// Exec Block Loop Only
					graph_exec_block_loop(graph);

					// Loop Recursive
					if(graph->start_loop)
						graph_exec(graph);
				}
			}
		}
		else
		{
			printf("[ERROR garph_exec] Can't find block loop\n");
		}
	}
	else
	{
		// Exec Blocks
		graph_exec_blocks(graph);
	}
}


// GET ROOTS

void graph_get_roots(t_graph *graph)
{
	t_context *C = ctx_get();

	t_link *l;
	t_block *block;
	t_brick *brick;
	t_plug *plug_in;

	if(!graph->roots)
	{
		t_node *node_list = scene_add(C->scene,nt_list,"roots");
		graph->roots = node_list->data;
	}

	t_lst *lst = graph->blocks;
	t_lst *roots = graph->roots;

	lst_cleanup(roots);

	for(l=lst->first;l;l=l->next)
	{
		t_link *link;
		block = l->data;
		int add_to_roots = 1;
		for(link=block->bricks->first;link;link=link->next)
		{
			brick=link->data;
			plug_in = &brick->plug_in;

			//if(plug_in->state.is_connected)
			if(plug_in->src)
			{
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

// BLOCK POS

void graph_set_block_pos(t_block *block, int pos)
{
	if(block->graph_pos < pos)
	{
		block->graph_pos = pos;
		t_link *link;

		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;
			if(brick->plug_out.state.is_connected)
			{
				t_plug *plug_dst = brick->plug_out.dst;
				t_brick *brick_dst = plug_dst->brick;
				t_block *block_dst = brick_dst->block;

				graph_set_block_pos(block_dst,pos+1);
			}
		}
	}
}

// SORT

void graph_sort(t_graph *graph)
{
	t_link *l;
	t_block *block;
	int pos;

	// Reset
	for(l=graph->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->graph_pos = 0;
	}

	// Set Pos
	for(l=graph->roots->first;l;l=l->next)
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

				graph_set_block_pos(block_dst,pos);
			}
		}
	}

	// Set Link order
	for(l=graph->blocks->first;l;l=l->next)
	{
		block=l->data;
		l->order = block->graph_pos;
	}
	
	// Sort List
	lst_sort_bubble(graph->blocks);
}

// SWAP

void graph_swap(t_graph *src, t_graph *dst)
{
	t_link *l;
	t_block *block;

	for(l=src->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->graph = dst;
		graph_block_add(dst, block);
	}
}

// MERGE

void graph_merge(t_graph *src, t_graph *dst)
{
	t_context *C = ctx_get();
	if(src->id == dst->id)
	{
		// nothing to do 
	}
	else
	{
		// New Graph
		scene_store(C->scene,1);
		t_graph *graph = graph_add("graph");
		scene_store(C->scene,0);

		// Merge Graphs
		graph_swap(src,graph);
		graph_swap(dst,graph);

		// Update New Graph
		graph_setup(graph);

		// Remove Old Graphs
		graph_delete(src);
		graph_delete(dst);
	}
}

// BUILD FROM LIST

void graph_build_from_list(t_lst *lst)
{
	t_context *C = ctx_get();
	t_link *l;
	t_block *block;

	// New Graph
	scene_store(C->scene,1);
	t_graph *new_graph = graph_add("graph");
	scene_store(C->scene,0);

	// Fill Graph
	for(l=lst->first;l;l=l->next)
	{
		block = l->data;
		graph_block_add(new_graph, block);
	}
}

void graph_draw_blocks(t_graph *graph)
{
	t_link *l;
	t_block *block;
	for(l=graph->blocks->first;l;l=l->next)
	{
		block = l->data;
		block->cls->draw(block);
	}
}

// DRAW BOUNDING BOX

void graph_draw_bounding_box(t_graph *graph)
{
	t_context *C = ctx_get();
	t_link *l;
	t_block *block;

	float up=0;
	float down=0;
	float left=0;
	float right=0;

	float margin = 10;

	int first_run = 1;

	if(graph->blocks)
	{
		for(l=graph->blocks->first;l;l=l->next)
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

// BLOCK ADD

void graph_block_add(t_graph *graph, t_block *block)
{
	t_context *C = ctx_get();

	// If Graph has Blocks Lst
	if(graph->blocks)		
	{
		int is_in_graph = 0;
		int id = block->id;
		t_link *l;
		t_block *b;

		// If Graph Has Blocks
		if(graph->blocks->first)
		{
			// Check Block Not In Graph yet (?)
			for(l=graph->blocks->first;l;l=l->next)
			{
				b = l->data;
				if(b->id == id)
				{
					is_in_graph = 1;
					break;
				}
			}

		}

		// Add to Graph
		if(!is_in_graph)
		{
			// Add to Graph List
			scene_store(C->scene,1);
			list_add(graph->blocks, block);
			block->graph = graph;
			scene_store(C->scene,0);

			// Pop from Set Blocks
			if(!block->state.is_in_graph) set_block_pop(block->set,block);

			// Set In Graph
			block->state.is_in_graph = 1;

			// Setup Graph
			graph_setup(graph);
		}
	}
	// Else Add Blocks Lst
	else
	{
		scene_store(C->scene,1);
		t_node *node_lst = scene_add(C->scene, nt_list, "lst");
		t_lst *lst = node_lst->data;
		graph->blocks = lst;
		scene_store(C->scene,0);

		// Add Block
		graph_block_add(graph, block);
	}
}

void graph_block_remove(t_graph *graph, t_block *block)
{
	// Reset Block State
	block->graph=NULL;
	block->state.is_in_graph = 0;

	// Add To Set
	set_block_push(block->set, block);

	// Setup
	graph_setup(graph);
}

// ADD

t_graph *graph_add(const char *name)
{
	t_context *C = ctx_get();

	// New Graph
	t_node *node_graph = scene_add(C->scene,nt_graph,"graph");
	t_graph *graph = node_graph->data;

	// Add To Set
	t_set *set = get_current_set(C);
	list_add(set->graphs,graph);

	graph->set = set;

	return graph;
}

// INIT

void graph_init(t_graph *graph)
{
	graph->start_loop = 0;
	graph->end_loop = 0;
}

// CLONE

t_graph *graph_clone(t_graph *graph)
{
	if(graph)
	{
		t_graph *clone = graph_new(graph->name);

		clone->blocks = lst_clone(graph->blocks, dt_block);
		clone->set = NULL;

		return clone;
	}
	else
	{
		return NULL;
	}
}


// REBIND

t_graph *graph_rebind(t_scene *sc,void *ptr)
{
	t_graph *graph=(t_graph *)ptr;

	rebind(sc,"graph","blocks",(void **)&graph->blocks);
	rebind(sc,"graph","set",(void **)&graph->set);

	return graph;
}

// FREE

void _graph_free(t_graph *graph)
{
	if(graph->blocks) _list_free(graph->blocks, dt_block);
	free(graph);
}

void graph_free(t_graph *graph)
{
	t_context *C = ctx_get();

	if(graph->blocks)
	{
		scene_struct_delete(C->scene,graph->blocks);
	}

	free(graph);
}

// NEW

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)malloc(sizeof(t_graph));

	graph->id = 0;
	graph->id_chunk = 0;
	graph->users = 0;
	set_name(graph->name, name);

	graph->blocks = NULL;
	graph->roots = NULL;

	graph->set = NULL;

	graph->has_loop = 0;
	graph->start_loop = 0;
	graph->end_loop = 0;
	graph->frame_based = 0;

	graph->done=0;

	return graph;
}



