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
		C->scene->store = 1;
		t_node *node_graph = scene_add(C->scene, nt_graph, "graph");
		t_graph *graph = node_graph->data;
		C->scene->store = 0;

		// Merge Graphs
		graph_swap(src,graph);
		graph_swap(dst,graph);

		// Remove Old Graphs
		scene_struct_delete(C->scene,src);
		scene_struct_delete(C->scene,dst);
	}
}

// BUILD FROM LIST

void graph_build_from_list(t_lst *lst)
{
	t_context *C = ctx_get();
	t_link *l;
	t_block *block;

	// New Graph
	C->scene->store = 1;
	t_node *node_graph = scene_add(C->scene, nt_graph, "graph");
	t_graph *new_graph = node_graph->data;
	C->scene->store = 0;

	for(l=lst->first;l;l=l->next)
	{
		block = l->data;
		graph_block_add(new_graph, block);
	}
}

// DRAW ROOTS

void graph_draw_roots(t_graph *graph)
{
	graph_get_roots(graph);
	if(graph->roots)
	{
		t_link *l = graph->roots->first;
		if(l)
		{
			for(;l;l=l->next)
			{
				t_block *block = l->data;
				glPushMatrix();
				glLoadIdentity();
				glScalef(1.1,1.1,1.1);
				block_draw_outline(block);
				glPopMatrix();
			}
		}
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
	if(graph->blocks)		
	{
		int is_in_graph = 0;
		int id = block->id;
		t_link *l;
		t_block *b;

		if(graph->blocks->first)
		{
			for(l=graph->blocks->first;l;l=l->next)
			{
				b = l->data;
				if(b->id == id)
				{
					is_in_graph = 1;
					break;
				}
			}

			if(!is_in_graph)
			{
				C->scene->store = 1;
				list_add(graph->blocks, block);
				block->graph = graph;
				C->scene->store = 0;
			}
		}
		else
		{
			C->scene->store = 1;
			list_add(graph->blocks, block);
			block->graph = graph;
			C->scene->store = 0;
		}
	}
	else
	{
		C->scene->store = 1;
		t_node *node_lst = scene_add(C->scene, nt_list, "lst");
		t_lst *lst = node_lst->data;
		graph->blocks = lst;
		C->scene->store = 0;

		graph_block_add(graph, block);
	}
}

// REBIND

t_graph *graph_rebind(t_scene *sc,void *ptr)
{
	t_graph *graph=(t_graph *)ptr;

	rebind(sc,"graph","blocks",(void **)&graph->blocks);

	return graph;
}

// FREE

void graph_free(t_graph *graph)
{
	t_context *C = ctx_get();
	if(graph->blocks)
	{

		scene_struct_delete(C->scene,graph->blocks);
	}
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

	return graph;
}



