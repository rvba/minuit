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

#include "graph.h"

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



