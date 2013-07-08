/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "common.h"

struct Lst;

typedef struct Dot
{
	void *data;

}t_dot;

typedef struct Dash
{
	t_dot *x;
	t_dot *y;

}t_dash;


typedef struct Graph
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	struct Lst *dots;
	struct Lst *dashes;

}t_graph;

void 		graph_show(t_graph *graph);
t_dot *	 	graph_dot_add(t_graph *graph, void *data);
t_dash *	graph_dash_add(t_graph *graph, t_dot *x, t_dot *y);
t_graph *	graph_make(const char *name);
t_graph *	graph_new(const char *name);
void 		graph_free(t_graph *graph);


#endif
