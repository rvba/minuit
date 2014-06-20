/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "mem.h"
#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "util.h"
#include "datum.h"
#include "data.h"


void datum_free(t_datum *datum)
{
	mem_free( datum->data, datum->size);
	mem_free( datum, sizeof( t_datum));
}

void datum_init( t_datum *datum)
{
}

void datum_rebind(t_scene *sc,void *ptr)
{
	t_datum *datum = (t_datum *) ptr;

	rebind(sc,"datum","data",(void **)&datum->data);
}

t_datum *datum_add( t_data_type type, const char *name, void *data)
{
	t_context *C = ctx_get();
	t_node *node = scene_add( C->scene, dt_datum, name);
	t_datum *datum = ( t_datum *) node->data;

	datum->type = type;

	switch( type)
	{
		case( dt_int):
			datum->data = (int *) mem_malloc( sizeof( int));

			if( C->scene->store)
			{
				scene_add_data_var( C->scene, "int", name, sizeof(int), datum->data);
			}

			if(data)
			{
				cprf_int( datum->data, data, 0);
			}	
			else
			{
				*( (int *) datum->data) = 0;
			}
			break;

		case( dt_float):
			datum->data = (float *) mem_malloc( sizeof( float));

			if( C->scene->store)
			{
				scene_add_data_var( C->scene, "float", name, sizeof(float), datum->data);
			}

			if(data)
			{
				cprf_float( datum->data, data, 0);
			}	
			else
			{
				*( (float *) datum->data) = 0;
			}
			break;



		default:
			printf("[ERROR datum_new] Unknown data type %s\n",data_name_get(type));
			break;
	}

	return datum;
}

void *datum_new( const char *name)
{
	t_datum *datum = ( t_datum *) mem_malloc( sizeof( t_datum));
	id_init( &datum->id, name);
	datum->type = dt_null;
	datum->data = NULL;

	return datum;
}
