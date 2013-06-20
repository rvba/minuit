/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SET_H
#define __SET_H

#include "common.h"

struct Scene;
struct Lst;
struct Block;

typedef struct Set
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	struct Lst *blocks;
	struct Lst *graphs;

	int frame_based;
	int process;
	int process_id;
	int processing;

}t_set;

void 		set_setup(t_set *set);
void 		set_exec(t_set *set);
void 		set_draw(t_set *set);
void 		set_block_pop(t_set *set, struct Block *block);
void 		set_block_push(t_set *set, struct Block *block);
t_set *		set_rebind(struct Scene *sc, void **ptr);
t_node *	set_add(const char *name);
t_set *		set_clone(t_set *set);
void 		_set_free(t_set *set);
void 		set_free(t_set *set);
t_node *	set_make(const char *name);
t_set *		set_new(const char *name);


#endif
