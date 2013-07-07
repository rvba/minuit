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
#include "node.h"
#include "context.h"
#include "scene.h"
#include "node.h"

g_node *g_node_new(void *data);
g_arc *g_arc_new(g_node *x, g_node *y);

// ADD

void graph_node_add(t_graph *graph, void *data)
{
	t_generic *g = (t_generic *) data;

	g_node *node = g_node_new(g->name);
	lst_add(graph->nodes, node, g->name);
}

void graph_arc_add(t_graph *graph, g_node *x, g_node *y)
{
	g_arc *arc = g_arc_new(x,y);
	lst_add(graph->arcs, arc, "arc");
}


// BUILD

void graph_build_from_list(t_lst *nodes, t_lst *arcs)
{
	t_graph *graph = graph_new("graph");
	t_link *link;

	// Add Nodes
	if(nodes)
	{
		for(link=nodes->first;link;link=link->next)
		{
			graph_node_add(graph, link->data);
		}
	}

	// Add Arcs
	if(arcs)
	{
		for(link=nodes->first;link;link=link->next)
		{
		}
	}
}

// FREE

void graph_free(t_graph *graph)
{
}

// MAKE

t_graph *graph_make(const char *name)
{
	t_context *C = ctx_get();

	t_graph *graph = graph_new(name);

	t_node *node = scene_add(C->scene,nt_list,"nlist");
	t_node *arc = scene_add(C->scene,nt_list,"alist");

	t_lst *n_list = node->data;
	t_lst *a_list = arc->data;

	graph->nodes = n_list;
	graph->arcs = a_list;

	return graph;
}

// NEW

g_node *g_node_new(void *data)
{
	g_node *node = (g_node *)malloc(sizeof(g_node));

	node->data = data;

	return node;
}

g_arc *g_arc_new(g_node *x, g_node *y)
{
	g_arc *arc = (g_arc *)malloc(sizeof(g_arc));

	arc->x = x;
	arc->y = y;

	return arc;
}

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)malloc(sizeof(t_graph));

	graph->id = 0;
	graph->id_chunk = 0;
	graph->users = 0;
	set_name(graph->name, name);

	graph->nodes = NULL;
	graph->arcs = NULL;

	return graph;
}
