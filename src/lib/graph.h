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
	int id;
	int root;
	int rank;
	struct Dot *parent;

	void *data;

}t_dot;

typedef struct Dash
{
	t_dot *x;
	t_dot *y;
	int id_x;
	int id_y;

}t_dash;


typedef struct Graph
{
	t_id id;

	struct Lst *dots;
	struct Lst *dashes;

	int dot_count;
	int dash_count;

}t_graph;


t_dot *		dot_new(void *data);
void 		dash_free(t_dash *dash);
t_dash *	dash_new(t_dot *x, t_dot *y);
void 		dot_show(t_dot *dot);

void 		graph_dj_set(t_graph *graph);
void 		graph_show(t_graph *graph);
void 		dash_show(t_dash *dash);
t_dot *		graph_dot_find(t_graph *graph, int id);
void 		graph_dash_remove(t_graph *graph, t_dot *x, t_dot *y);
void 		graph_link_remove(t_graph *graph, int id_x, int id_y);
int 		graph_dash_exists(t_graph *graph, t_dot *dot_x, t_dot *dot_y);
int 		graph_link_exists(t_graph *graph, int id_x, int id_y);
t_dot *	 	graph_dot_add(t_graph *graph, void *data);
t_dash *	graph_dash_add(t_graph *graph, t_dot *x, t_dot *y);
t_graph *	graph_make(const char *name);
t_graph *	graph_new(const char *name);
void 		graph_free(t_graph *graph);


#endif
