/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __GRAPH_H
#define __GRAPH_H

#include "common.h"

struct Scene;
struct Block;

typedef struct Graph
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	struct Lst *blocks;
	struct Lst *roots;

	struct Set *set;

	int has_loop;
	int start_loop;
	int end_loop;

}t_graph;

void 		graph_delete(t_graph *graph);
void 		graph_exec(t_graph *graph);
void 		graph_sort(t_graph *graph);
void 		graph_get_roots(t_graph *graph);
void 		graph_build_from_list(t_lst *lst);
void 		graph_merge(t_graph *src, t_graph *dst);
void 		graph_draw_roots(t_graph *graph);
void 		graph_draw_blocks(t_graph *graph);
void 		graph_draw_bounding_box(t_graph *graph);
void 		graph_block_add(t_graph *graph, struct Block *block);
void 		graph_block_remove(t_graph *graph, struct Block *block);
t_graph *	graph_add(const char *name);
void 		graph_init(t_graph *graph);
t_graph *	graph_rebind(struct Scene *sc,void *ptr);
void 		graph_free(t_graph *graph);
t_graph *	graph_new(const char *name);


#endif
