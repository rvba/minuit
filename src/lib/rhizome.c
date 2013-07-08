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


void rhizome_graph_block_add(t_rhizome *rhizome, t_block *block)
{
	block->dot = graph_dot_add(rhizome->graph, block);
}

void rhizome_graph_dash_add(t_rhizome *rhizome, t_block *block_x, t_block *block_y)
{
	t_graph *graph = block_x->rhizome->graph;
	t_dot *dot_x = block_x->dot;
	t_dot *dot_y = block_y->dot;
	graph_dash_add(graph, dot_x, dot_y);

	graph_show(graph);
}

void rhizome_setup(t_rhizome *rhizome)
{
	t_link *link;
	t_block *block;
	int has_loop = 0;
	int frame_based = 0;

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
}

void rhizome_draw(t_rhizome *rhizome)
{
	rhizome_draw_blocks(rhizome);
	rhizome_draw_bounding_box(rhizome);
	rhizome_get_roots(rhizome);
	rhizome_sort(rhizome);
}

void rhizome_delete(t_rhizome *rhizome)
{
	t_context *C = ctx_get();
	t_set *set = rhizome->set;
	// Remove From Set
	list_remove_by_id(set->rhizomes,rhizome->id);
	// Struct Delete
	scene_struct_delete(C->scene,rhizome);
}

// LOOP

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

// Exec Graph
void rhizome_exec(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	// Setup
	rhizome_setup(rhizome);

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
				rhizome->done = 1;
			}
			// Start Loop
			else
			{
				// Loop
				if(rhizome->done)
				{
					// Exit
					rhizome->done=0;
				}
				else
				{
					// Exec Block Loop Only
					rhizome_exec_block_loop(rhizome);

					// Loop Recursive
					if(rhizome->start_loop)
						rhizome_exec(rhizome);
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
		rhizome_exec_blocks(rhizome);
	}
}


// GET ROOTS

void rhizome_get_roots(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	t_link *l;
	t_block *block;
	t_brick *brick;
	t_plug *plug_in;

	if(!rhizome->roots)
	{
		t_node *node_list = scene_add(C->scene,nt_list,"roots");
		rhizome->roots = node_list->data;
	}

	t_lst *lst = rhizome->blocks;
	t_lst *roots = rhizome->roots;

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
	if(src->id == dst->id)
	{
		// nothing to do 
	}
	else
	{
		// New Graph
		scene_store(C->scene,1);
		t_rhizome *rhizome = rhizome_add("rhizome");
		scene_store(C->scene,0);

		// Merge Graphs
		rhizome_swap(src,rhizome);
		rhizome_swap(dst,rhizome);

		// Update New Graph
		rhizome_setup(rhizome);

		// Remove Old Graphs
		rhizome_delete(src);
		rhizome_delete(dst);
	}
}

// BUILD FROM LIST

void rhizome_build_from_list(t_lst *lst)
{
	t_context *C = ctx_get();
	t_link *l;
	t_block *block;

	// New Graph
	scene_store(C->scene,1);
	t_rhizome *new_rhizome = rhizome_add("rhizome");
	scene_store(C->scene,0);

	// Fill Graph
	for(l=lst->first;l;l=l->next)
	{
		block = l->data;
		rhizome_block_add(new_rhizome, block);
	}
}

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

// BLOCK ADD

void rhizome_block_add(t_rhizome *rhizome, t_block *block)
{
	t_context *C = ctx_get();

	// If Graph has Blocks Lst
	if(rhizome->blocks)		
	{
		int is_in_rhizome = 0;
		int id = block->id;
		t_link *l;
		t_block *b;

		// If Graph Has Blocks
		if(rhizome->blocks->first)
		{
			// Check Block Not In Graph yet (?)
			for(l=rhizome->blocks->first;l;l=l->next)
			{
				b = l->data;
				if(b->id == id)
				{
					is_in_rhizome = 1;
					break;
				}
			}

		}

		// Add to Graph
		if(!is_in_rhizome)
		{
			// Add to Graph List
			scene_store(C->scene,1);
			list_add(rhizome->blocks, block);
			block->rhizome = rhizome;
			scene_store(C->scene,0);

			// Pop from Set Blocks
			if(!block->state.is_in_rhizome) set_block_pop(block->set,block);

			// Set In Graph
			block->state.is_in_rhizome = 1;

			// Setup Graph
			rhizome_setup(rhizome);
		}

		// Add To Graph
		rhizome_graph_block_add(rhizome, block);
	}
	// Else Add Blocks Lst
	else
	{
		scene_store(C->scene,1);
		t_node *node_lst = scene_add(C->scene, nt_list, "lst");
		t_lst *lst = node_lst->data;
		rhizome->blocks = lst;
		scene_store(C->scene,0);

		// Add Block
		rhizome_block_add(rhizome, block);
	}

}

// BLOCK REMOVE

void rhizome_block_remove(t_rhizome *rhizome, t_block *block)
{
	// Reset Block State
	block->rhizome=NULL;
	block->state.is_in_rhizome = 0;

	// Add To Set
	set_block_push(block->set, block);

	// Setup
	rhizome_setup(rhizome);
}

// ADD

t_rhizome *rhizome_add(const char *name)
{
	t_context *C = ctx_get();

	// New Rhizome
	t_node *node_rhizome = scene_add(C->scene,nt_rhizome,"rhizome");
	t_rhizome *rhizome = node_rhizome->data;

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

void rhizome_init(t_rhizome *rhizome)
{
	rhizome->start_loop = 0;
	rhizome->end_loop = 0;
}

// CLONE

t_rhizome *rhizome_clone(t_rhizome *rhizome)
{
	if(rhizome)
	{
		t_rhizome *clone = rhizome_new(rhizome->name);

		clone->blocks = lst_clone(rhizome->blocks, dt_block);
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

	return rhizome;
}

// FREE

void _rhizome_free(t_rhizome *rhizome)
{
	if(rhizome->blocks) _list_free(rhizome->blocks, dt_block);
	free(rhizome);
}

void rhizome_free(t_rhizome *rhizome)
{
	t_context *C = ctx_get();

	if(rhizome->blocks)
	{
		scene_struct_delete(C->scene,rhizome->blocks);
	}

	free(rhizome);
}

// NEW

t_rhizome *rhizome_new(const char *name)
{
	t_rhizome *rhizome = (t_rhizome *)malloc(sizeof(t_rhizome));

	rhizome->id = 0;
	rhizome->id_chunk = 0;
	rhizome->users = 0;
	set_name(rhizome->name, name);

	rhizome->blocks = NULL;
	rhizome->roots = NULL;

	rhizome->set = NULL;
	rhizome->graph = NULL;

	rhizome->has_loop = 0;
	rhizome->start_loop = 0;
	rhizome->end_loop = 0;
	rhizome->frame_based = 0;

	rhizome->done=0;

	return rhizome;
}



