/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

void cls_vector_init(t_vector *vector)
{
	vector->data = NULL;
}

t_vector_cls cls_vector=
{
	.init=cls_vector_init
};

void vector_free(t_vector *vector)
{
}

void vector_show(t_vector *vector)
{
	t_context *C = ctx_get();

	if(C->event->debug_console)
	{
		term_log( "vector %s\n", vector->name);
		term_log( "type %s\n", data_name_get(vector->type));
		term_log( "length %d\n", vector->length);
		term_log( "data %p\n", vector->data);
		if(vector->data)
		{
			int *data_int;

			switch(vector->type)
			{
				case dt_float:
					switch(vector->length)
					{
						case 3:
							data_int = vector->data;
							term_log( "[v3] %f %f %f",
								drf_float(data_int),
								drf_float(data_int+1),
								drf_float(data_int+2)
								);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}

	if(C->event->debug_terminal)
	{
		printf( "vector %s\n", vector->name);
		printf( "type %s\n", data_name_get(vector->type));
		printf( "length %d\n", vector->length);
		printf( "data %p\n", vector->data);
		printf( "vlst %p\n", vector->vlst);
	}

	if(vector->vlst) vlst_show(vector->vlst);
}

void vector_op_add(t_vector *dst,t_vector *src)
{
	t_vlst *vlst_src = src->vlst;
	t_vlst *vlst_dst = dst->vlst;

	float *data_src = vlst_src->data;
	float *data_dst = vlst_dst->data;

	float result[] = {0,0,0};

	// Copy Vlst Data
	vadd(result,data_src,data_dst);
	vset3f(data_dst,result[0],result[1],result[2]);
}

void vector_op_copy(t_vector *dst, t_vector *src)
{
	dst->type = src->type;
	dst->length = src->length;
	dst->data = src->data;

	t_vlst *vlst_src = src->vlst;
	t_vlst *vlst_dst = dst->vlst;

	float *v_src = vlst_src->data;
	float *v_dst = vlst_dst->data;

	vcp(v_dst,v_src);
}

void vector_default(t_vector *vector)
{
	vector->length = 3;
	vector->type = dt_float;
	vector->vlst = vlst_make("name",_3f,1);
	vset3f(vector->vlst->data,0,0,0);
}

t_node *vector_add(const char *name)
{
	t_context *C = ctx_get();
	t_node *node = scene_add(C->scene,nt_vector,name);
	t_vector *vector = node->data;

	vector_default(vector);

	return node;
}

t_vector *vector_rebind(t_scene *sc,void *ptr)
{
	t_vector *vector = (t_vector *)ptr;

	check_init("VECTOR",vector->name);

	rebind(sc,"vector","vlst",(void **)&vector->vlst);

	return vector;
}

t_vector *vector_new(const char *name)
{
	t_vector *vector = (t_vector *) malloc(sizeof(t_vector));

	vector->id=0;
	vector->id_chunk=0;
	set_name(vector->name,name);
	vector->users=0;

	vector->type=dt_null;
	vector->length=0;
	vector->data=NULL;

	vector->cls=&cls_vector;

	return vector;
}
