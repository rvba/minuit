/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "node.h"
#include "scene.h"

#include "op.h"
#include "event.h"
#include "term.h"
#include "vector.h"
#include "list.h"
#include "vlst.h"
#include "ui.h"
#include "data.h"
#include "memory.h"

void cls_vector_init(t_vector *vector)
{
	vector->pointer = NULL;
}

t_vector_cls cls_vector=
{
	.init=cls_vector_init
};

void vector_free(t_vector *vector)
{
	if(vector->vector) vlst_delete(vector->vector);
}

void vector_delete( t_vector *vector)
{
	t_scene *sc = ctx_scene_get();
	scene_delete( sc, vector);
}

void vector_copy_pointer(t_vector *dst, t_vector *src)
{
	dst->pointer = src->pointer;
	dst->length = src->length;
	dst->type = src->type;

	//XXX vector cls !!!
}
	

void *vector_get_pointer(t_vector *vector, int indice)
{
	void *ptr = vector->pointer;

	switch(vector->type)
	{
		case(dt_float): return grf_float(ptr, indice); break;
		case(dt_uint): return grf_uint(ptr, indice); break;
		default: 
			printf("[ERR vector_get_pointer] Unknown type %s\n",data_name_get(vector->type));
			return NULL;
			break;
	}
}

int vector_is_different(t_vector *dst, t_vector *src)
{
	if(src->length != dst->length)
		return 1;
	else if(src->type != dst->type)
		return 1;
	else
		return 0;
}

void vector_show(t_vector *vector)
{
	t_context *C = ctx_get();

	if(C->event->debug_console)
	{
		term_log( "vector %s\n", vector->id.name);
		term_log( "type %s\n", data_name_get(vector->type));
		term_log( "length %d\n", vector->length);
		term_log( "pointer %p\n", vector->pointer);
		if(vector->pointer)
		{
			int *data_int;

			switch(vector->type)
			{
				case dt_float:
					switch(vector->length)
					{
						case 3:
							data_int = vector->pointer;
							term_log( "pointer [v3] %f %f %f",
								drf_float(data_int),
								drf_float(data_int+1),
								drf_float(data_int+2)
								);
							break;
						default:
							printf("[VECTOR SHOW FLOAT] Unknwon length %d\n",vector->length);
							break;
					}
					break;
				default:
					printf("[VECTOR SHOW] Unkown type \n");
					break;
			}
		}
	}

	if(C->event->debug_terminal)
	{
		printf( "vector %s\n", vector->id.name);
		printf( "type %s\n", data_name_get(vector->type));
		printf( "length %d\n", vector->length);
		printf( "pointer %p\n", vector->pointer);
		printf( "vector %p\n", vector->vector);

		if(vector->pointer)
		{
			int *data_int;

			switch(vector->type)
			{
				case dt_float:
					switch(vector->length)
					{
						case 3:
							data_int = vector->pointer;
							printf( "pointer [v3] %f %f %f\n",
								drf_float(data_int),
								drf_float(data_int+1),
								drf_float(data_int+2)
								);
							break;
						default:
							printf("[VECTOR SHOW FLOAT] Unknwon length %d\n",vector->length);
							break;
					}
					break;
				default:
					printf("[VECTOR SHOW] Unkown type \n");
					break;
			}
		}
	}

	if(vector->vector) vlst_show(vector->vector);
}

void vector_op_add(t_vector *dst,t_vector *src)
{
	t_vlst *vlst_src = src->vector;
	t_vlst *vlst_dst = dst->vector;

	float *data_src = vlst_src->data;
	float *data_dst = vlst_dst->data;

	t_context *C = ctx_get();

	if(dst->pointer)
	{
		if(src->pointer)
		{
			if(C->ui->show_step) term_log("src ptr dst ptr");
			vadd(dst->pointer, dst->pointer, src->pointer);
		}
		else
		{
			if(C->ui->show_step) term_log("src ptr dst data");
			vadd(dst->pointer, dst->pointer, data_src);
		}
	}
	else
	{
		if(src->pointer)
		{
			if(C->ui->show_step) term_log("src ptr dst data");
			vadd(data_dst,data_dst, src->pointer);
		}
		else
		{
			if(C->ui->show_step) term_log("src data dst data");
			vadd(data_dst,data_dst, data_src);
		}
	}
}

void vector_default(t_vector *vector)
{
	vector->length = 3;
	vector->type = dt_float;
	vector->vector = vlst_make("name", dt_float, 3, 1, NULL);
	vset3f(vector->vector->data,0,0,0);
}

t_node *vector_add(const char *name)
{
	t_context *C = ctx_get();
	t_node *node = scene_add(C->scene,dt_vector,name);
	t_vector *vector = node->data;

	vector_default(vector);

	return node;
}

void vector_rebind(t_scene *sc,void *ptr)
{
	t_vector *vector = (t_vector *)ptr;

	rebind(sc,"vector","vlst",(void **)&vector->vector);
}

void *vector_new(const char *name)
{
	t_vector *vector = (t_vector *) mem_malloc(sizeof(t_vector));

	id_init(&vector->id, name);

	vector->type=dt_null;
	vector->length=0;
	vector->pointer=NULL;

	vector->cls=&cls_vector;

	return vector;
}
