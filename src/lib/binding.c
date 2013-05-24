/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

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
	free(binding);
}

// REBIND

t_binding *binding_rebind(t_scene *sc, void *ptr)
{
	t_binding *binding = (t_binding *) ptr;

	check_init("BINDING", binding->name);

	rebind(sc, "binding", "data", (void **) &binding->data);

	check_check("BINDING", binding->name);

	return binding;
}

// NEW

t_binding *binding_new(const char *name)
{
	t_binding *binding=(t_binding *)malloc(sizeof(t_binding));

	binding->id=0;
	binding->id_chunk=0;
	set_name(binding->name,name);
	binding->users=0;
	binding->type=dt_null;

	binding->data = NULL;

	return binding;
}
