/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "util.h"
#include "scene.h"
#include "op.h"
#include "binding.h"
#include "data.h"
#include "memory.h"
#include "ctx.h"

// UPDATE

void binding_update(t_binding *binding, void *ptr)
{
	switch(binding->type)
	{
		case(dt_int):
			set_int(binding->data,drf_int(ptr));
			break;

		case(dt_uint):
			set_uint(binding->data,drf_uint(ptr));
			break;
		default:
			printf("[binding update] %s Not Implemented\n",data_name_get(binding->type));
			break;
	}
}

// FREE

void binding_free(t_binding *binding)
{
	mem_free( binding, sizeof( t_binding));
}

// REBIND

void binding_rebind(t_scene *sc, void *ptr)
{
	t_binding *binding = (t_binding *) ptr;

	rebind(sc, "binding", "data", (void **) &binding->data);
}

// NEW

void *binding_new(const char *name)
{
	t_binding *binding=(t_binding *)mem_malloc(sizeof(t_binding));

	id_init(&binding->id, name);
	binding->type=dt_null;

	binding->data = NULL;

	return binding;
}
