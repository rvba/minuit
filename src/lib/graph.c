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

//t_dot *dot_new(void *data);
void dash_free(t_dash *dash);
t_dash *dash_new(t_dot *x, t_dot *y);

// DISJOIN

t_dot *graph_dj_find(t_dot *dot)
{
	if(dot->parent == dot)
	{
		return dot;
	}
	else
	{
		return (dot->parent = graph_dj_find(dot->parent));
	}
}

void graph_dj_union(t_dot *x, t_dot *y)
{
	t_dot *root_x = graph_dj_find(x);
	t_dot *root_y = graph_dj_find(y);

	if(root_x->rank > root_y->rank)
	{
		root_y->parent = root_x;
	}
	else if(root_x->rank < root_y->rank)
	{
		root_x->parent = root_y;
	}
	else
	{
		root_x->parent = root_y;
		root_y->rank++;
	}
}

void graph_dj_set(t_graph *graph)
{
	t_link *link;
	t_dot *dot;
	t_dash *dash;

	// Init, Set Dot To Be Parent Of Itself
	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;

		dot->parent = dot;
		dot->root = dot->id;
		dot->rank = 0;
	}

	// Unite sets
	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;	
		graph_dj_union(dash->x, dash->y);
	}

	// Set Root Id
	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;
		dot->root = dot->parent->root;
	}
}

// SHOW

void dash_show(t_dash *dash)
{
	t_generic *x = (t_generic *)dash->x->data;
	t_generic *y = (t_generic *)dash->y->data;

	printf("dash %d:%d (%s)->(%s)\n",dash->id_x, dash->id_y,x->name,y->name);
}

void dot_show(t_dot *dot)
{
	t_generic *g = (t_generic *) dot->data;
	printf("dot data:%s id:%d root:%d\n",g->name,dot->id, dot->root);
}

void graph_show(t_graph *graph)
{
	t_link *link;
	t_dash *dash;
	t_dot *dot;

	printf("graph (%s)\n",graph->name);
	printf("DOTS\n");
	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;
		dot_show(dot);
	}
	printf("DASHES\n");
	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		dash_show(dash);
	}
}

// DASH EXISTS

int graph_dash_exists(t_graph *graph, t_dot *dot_x, t_dot *dot_y)
{
	t_link *link;
	t_dash *dash;
	t_generic *s_x;
	t_generic *s_y;
	t_generic *g_x;
	t_generic *g_y;

	s_x = (t_generic *) dot_x->data;
	s_y = (t_generic *) dot_y->data;

	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		g_x = (t_generic *)dash->x->data;
		g_y = (t_generic *)dash->y->data;

		if((s_x->id == g_x->id) && (s_y->id == g_y->id))
		{
			return 1;
		}
	}

	return 0;
}

int graph_link_exists(t_graph *graph, int id_x, int id_y)
{
	t_link *link;
	t_dash *dash;

	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		int dash_x = dash->id_x;
		int dash_y = dash->id_y;

		if(((id_x == dash_x) && (id_y == dash_y)) || ((id_x == dash_y) && (id_y == dash_x)))
		{
			return 1;
		}
	}

	return 0;
}

// FIND

t_dash *graph_dash_find(t_graph *graph, t_dot *x, t_dot *y)
{
	t_link *link;
	t_dash *dash;
	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		if(
			((dash->x->id == x->id) && (dash->y->id == y->id))
			||
			((dash->y->id == x->id) && (dash->x->id == y->id))
			)
		{
			return dash;
		}
	}
	return NULL;
}

t_dash *graph_link_find(t_graph *graph, int id_x, int id_y)
{
	t_link *link;
	t_dash *dash;
	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		int dash_x = dash->id_x;
		int dash_y = dash->id_y;

		if(((id_x == dash_x) && (id_y == dash_y)) || ((id_x == dash_y) && (id_y == dash_x)))
		{
			return dash;
		}
	}
	return NULL;
}

t_dot *graph_dot_find(t_graph *graph, int id)
{
	t_link *link;
	t_dot *dot;

	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;
		if(dot->id == id)
		{
			return dot;
		}
	}

	return NULL;
}


// REMOVE

void graph_dash_remove(t_graph *graph, t_dot *x, t_dot *y)
{
	t_dash *dash = graph_dash_find(graph,x,y); 
	if(dash)
	{
		lst_remove_by_ptr(graph->dashes, dash);
		graph->dash_count--;
		dash_free(dash);
	}
}

void graph_link_remove(t_graph *graph, int id_x, int id_y)
{
	t_dash *dash = graph_link_find(graph,id_x,id_y); 
	if(dash)
	{
		lst_remove_by_ptr(graph->dashes, dash);
		graph->dash_count--;
		dash_free(dash);
	}
}

// ADD

t_dot *graph_dot_add(t_graph *graph, void *data)
{
	t_generic *g = (t_generic *) data;

	t_dot *dot = dot_new(data);
	lst_add(graph->dots, dot, g->name);

	graph->dot_count++;

	return dot;
}

t_dash *graph_dash_add(t_graph *graph, t_dot *x, t_dot *y)
{
	t_dash *dash = dash_new(x,y);
	lst_add(graph->dashes, dash, "dash");
	graph->dash_count++;

	return dash;
}


// FREE

void dash_free(t_dash *dash)
{
	free(dash);
}

void graph_free(t_graph *graph)
{
}

// MAKE

t_graph *graph_make(const char *name)
{
	t_graph *graph = graph_new(name);

	t_lst *dot_list = lst_new("dots");
	t_lst *dash_list = lst_new("dashs");

	graph->dots = dot_list;
	graph->dashes = dash_list;

	return graph;
}

// NEW

t_dot *dot_new(void *data)
{
	t_dot *dot = (t_dot *)malloc(sizeof(t_dot));

	t_generic *g = (t_generic *) data;

	dot->id = g->id;
	dot->root = 0;
	dot->rank = 0;
	dot->parent = NULL;
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

	graph->dot_count = 0;
	graph->dash_count = 0;

	return graph;
}
