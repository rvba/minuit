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

typedef struct G_node
{
	void *data;

}g_node;

typedef struct G_arc
{
	g_node *x;
	g_node *y;

}g_arc;


typedef struct Graph
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	struct Lst *nodes;
	struct Lst *arcs;

}t_graph;

t_graph *graph_new(const char *name);
void graph_free(t_graph *graph);


#endif
