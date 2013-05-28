/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __BINDING_H
#define __BINDING_H

#include "common.h"
#include "data.h"

struct Scene;


typedef struct Binding
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	enum Data_Type type;

	void *data;
}t_binding;

void binding_update(t_binding *binding, void *ptr);
t_binding *binding_rebind(struct Scene *sc, void *ptr);
t_binding *binding_new(const char *name);
void binding_free(t_binding *binding);


#endif
