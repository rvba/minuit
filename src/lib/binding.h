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

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;

typedef struct Binding
{
	t_id id;
	enum Data_Type type;
	void *data;

}t_binding;

void binding_update(t_binding *binding, void *ptr);
void binding_rebind(struct Scene *sc, void *ptr);
void *binding_new(const char *name);
void binding_free(t_binding *binding);

#ifdef __cplusplus
}
#endif


#endif
