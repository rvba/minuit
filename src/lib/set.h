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

#include "minuit.h"

typedef struct Set t_set;

struct Set
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	t_lst *lst;
};

t_set *set_rebind(t_scene *sc, void **ptr);
t_node *set_add(const char *name);
void set_free(t_set *set);
t_node *set_make(const char *name);
t_set *set_new(const char *name);


#endif
