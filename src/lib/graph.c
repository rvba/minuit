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
#include "list.h"

#include "graph.h"

void graph_show(t_graph *graph)
{
	if(graph->blocks)
	{
		lst_show(graph->blocks);
	}
	else
	{
		printf("no blocks!\n");
	}
}

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
				C->scene->store = 0;
			}
		}
		else
		{
			C->scene->store = 1;
			list_add(graph->blocks, block);
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

void graph_free(t_graph *graph)
{
	t_context *C = ctx_get();
	if(graph->blocks)
	{
		scene_struct_delete(C->scene,graph->blocks);
	}
}

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)malloc(sizeof(t_graph));

	graph->id = 0;
	graph->id_chunk = 0;
	graph->users = 0;
	set_name(graph->name, name);

	graph->blocks = NULL;

	return graph;
}



