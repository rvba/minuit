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
#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "memory.h"


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
		dot->root = dot->i;
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
		t_dot *root = graph_dj_find(dot);
		dot->root = root->i;
	}
}

// SHOW

void dash_show(t_dash *dash)
{
	t_id *x = (t_id *) dash->x->data;
	t_id *y = (t_id *) dash->y->data;

	printf("dash %d:%d (%s)->(%s)\n",dash->i_x, dash->i_y,x->name,y->name);
}

void dot_show(t_dot *dot)
{
	t_id *id = (t_id *) dot->data;
	printf("dot data:%s id:%d root:%d\n",id->name, dot->i, dot->root);

	if(dot->parent)
	{
		t_dot *parent = dot->parent;
		t_id *_id = (t_id *) parent->data;
		printf("> parent %s\n",_id->name);
	}
	printf("\n");
}

void graph_show(t_graph *graph)
{
	t_link *link;
	t_dash *dash;
	t_dot *dot;

	printf("graph (%s)\n", graph->id.name);
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

int graph_link_exists(t_graph *graph, int i_x, int i_y)
{
	t_link *link;
	t_dash *dash;

	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		int dash_x = dash->i_x;
		int dash_y = dash->i_y;

		if(((i_x == dash_x) && (i_y == dash_y)) || ((i_x == dash_y) && (i_y == dash_x)))
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
			((dash->x->i == x->i) && (dash->y->i == y->i))
			||
			((dash->y->i == x->i) && (dash->x->i == y->i))
			)
		{
			return dash;
		}
	}
	return NULL;
}

t_dash *graph_link_find(t_graph *graph, int i_x, int i_y)
{
	t_link *link;
	t_dash *dash;
	for(link=graph->dashes->first;link;link=link->next)
	{
		dash = link->data;
		int dash_x = dash->i_x;
		int dash_y = dash->i_y;

		if(((i_x == dash_x) && (i_y == dash_y)) || ((i_x == dash_y) && (i_y == dash_x)))
		{
			return dash;
		}
	}
	return NULL;
}

t_dot *graph_dot_find(t_graph *graph, int i)
{
	t_link *link;
	t_dot *dot;

	for(link=graph->dots->first;link;link=link->next)
	{
		dot = link->data;
		if(dot->i == i)
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
		lst_link_delete_by_ptr(graph->dashes, dash);
		graph->dash_count--;
		dash_free(dash);
	}
}

void graph_link_remove(t_graph *graph, int i_x, int i_y)
{
	t_dash *dash = graph_link_find(graph,i_x,i_y); 
	if(dash)
	{
		lst_link_delete_by_ptr(graph->dashes, dash);
		graph->dash_count--;
		dash_free(dash);
	}
}

// ADD

t_dot *graph_dot_add(t_graph *graph, void *data)
{
	t_id *id = (t_id *) data;

	t_dot *dot = dot_new(data);
	lst_add(graph->dots, dot, id->name);

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
	mem_free( dash, sizeof( t_dash));
}

void dot_free(t_dot *dot)
{
	mem_free( dot, sizeof( t_dot));
}

void graph_free_dots(t_lst *lst)
{
	t_link *l;
	t_dot *dot;
	for(l=lst->first;l;l=l->next)
	{
		dot = l->data;
		dot_free(dot);
	}
}

void graph_free_dashs(t_lst *lst)
{
	t_link *l;
	t_dash *dash;
	for(l=lst->first;l;l=l->next)
	{
		dash = l->data;
		dash_free(dash);
	}
}


void graph_free(t_graph *graph)
{
	if(graph->dots) graph_free_dots(graph->dots);
	if(graph->dashes) graph_free_dashs(graph->dashes);
	lst_free(graph->dots);
	lst_free(graph->dashes);

	mem_free( graph, sizeof( t_graph));
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
	t_dot *dot = (t_dot *)mem_malloc(sizeof(t_dot));

	t_id *id = (t_id *) data;

	dot->i = id->id;
	dot->root = 0;
	dot->rank = 0;
	dot->parent = NULL;
	dot->data = data;

	return dot;
}

t_dash *dash_new(t_dot *x, t_dot *y)
{
	t_dash *dash = (t_dash *)mem_malloc(sizeof(t_dash));

	dash->x = x;
	dash->y = y;

	return dash;
}

t_graph *graph_new(const char *name)
{
	t_graph *graph = (t_graph *)mem_malloc(sizeof(t_graph));

	id_init(&graph->id, name);

	graph->dots = NULL;
	graph->dashes = NULL;

	graph->dot_count = 0;
	graph->dash_count = 0;

	return graph;
}
