/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "list.h"
#include "graph.h"

t_gvertex *gvertex_new(void *data);

// BUILD

void graph_build(t_graph *graph, t_lst *list)
{
	t_link *link;

	// New List
	graph->vertices = lst_new("vertices");

	// Add Vertices
	if(list)
	{
		for(link=list->first;link;link=link->next)
		{
			t_gvertex *gvertex = gvertex_new(link->data);
			lst_add(graph->vertices,gvertex,"gvertex");
		}
	}

}

// FREE

void graph_free(t_graph *graph)
{
}

// NEW

t_gvertex *gvertex_new(void *data)
{
	t_gvertex *gvertex = (t_gvertex *)malloc(sizeof(t_gvertex));
	gvertex->data = data;

	return gvertex;
}

t_gedge *gedge_new(t_gvertex *x, t_gvertex *y)
{
	t_gedge *gedge = (t_gedge *)malloc(sizeof(t_gedge));
	gedge->x = x;
	gedge->y = y;

	return gedge;
}

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)malloc(sizeof(t_graph));

	graph->id = 0;
	graph->id_chunk = 0;
	graph->users = 0;
	set_name(graph->name, name);

	graph->vertices = NULL;
	graph->edges = NULL;

	return graph;
}
