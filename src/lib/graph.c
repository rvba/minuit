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

t_dot *dot_new(void *data);
t_dash *dash_new(t_dot *x, t_dot *y);

// ADD

t_dot *graph_dot_add(t_graph *graph, void *data)
{
	t_generic *g = (t_generic *) data;

	t_dot *dot = dot_new(g->name);
	lst_add(graph->dots, dot, g->name);

	return dot;
}

t_dash *graph_dash_add(t_graph *graph, t_dot *x, t_dot *y)
{
	t_dash *dash = dash_new(x,y);
	lst_add(graph->dashes, dash, "dash");

	return dash;
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

	t_node *dot = scene_add(C->scene,nt_list,"dot_list");
	t_node *dash = scene_add(C->scene,nt_list,"dash_list");

	t_lst *dot_list = dot->data;
	t_lst *dash_list = dash->data;

	graph->dots = dot_list;
	graph->dashes = dash_list;

	return graph;
}

// NEW

t_dot *dot_new(void *data)
{
	t_dot *dot = (t_dot *)malloc(sizeof(t_dot));

	dot->data = data;

	return dot;
}

t_dash *dash_new(t_dot *x, t_dot *y)
{
	t_dash *dash = (t_dash *)malloc(sizeof(t_dash));

	dash->x = x;
	dash->y = y;

	return dash;
}

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)malloc(sizeof(t_graph));

	graph->id = 0;
	graph->id_chunk = 0;
	graph->users = 0;
	set_name(graph->name, name);

	graph->dots = NULL;
	graph->dashes = NULL;

	return graph;
}
