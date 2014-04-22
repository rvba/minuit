/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __RHIZOME_H
#define __RHIZOME_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct Block;
struct Set;

typedef struct Rhizome
{
	t_id id;

	struct Lst *blocks;
	struct Lst *roots;

	struct Set *set;
	struct Graph *graph;

	int has_loop;
	int frame_based;
	int start_loop;
	int end_loop;

}t_rhizome;


void 		rhizome_setup(t_rhizome *rhizome);
void 		rhizome_graph_split(t_rhizome *rhizome, struct Brick *brick_x, struct Brick *brick_y);
void 		rhizome_graph_link_add(t_rhizome *rhizome, struct Brick *brick_x, struct Brick *brick_y);
void 		rhizome_draw(t_rhizome *rhizome);
void 		rhizome_delete(t_rhizome *rhizome);
void 		rhizome_exec(t_rhizome *rhizome);
void 		rhizome_sort(t_rhizome *rhizome);
void 		rhizome_get_roots(t_rhizome *rhizome);
t_rhizome *	rhizome_build_from_list(t_lst *lst);
void 		rhizome_merge(t_rhizome *src, t_rhizome *dst);
void 		rhizome_draw_roots(t_rhizome *rhizome);
void 		rhizome_draw_blocks(t_rhizome *rhizome);
void 		rhizome_draw_bounding_box(t_rhizome *rhizome);
void 		rhizome_block_add(t_rhizome *rhizome, struct Block *block);
void 		rhizome_block_reset(struct Block *block);
void 		rhizome_block_remove(t_rhizome *rhizome, struct Block *block);
t_rhizome *	rhizome_add(const char *name);

void rhizome_graph_build(t_rhizome *rhizome);
void 		rhizome_init(t_rhizome *rhizome);
void		rhizome_rebind(struct Scene *sc,void *ptr);

t_rhizome *	rhizome_clone(t_rhizome *rhizome);
void 		_rhizome_free(t_rhizome *rhizome);
void 		rhizome_free(t_rhizome *rhizome);
t_rhizome *	rhizome_new(const char *name);

#ifdef __cplusplus
}
#endif

#endif
