/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "node.h"
#include "scene.h"
#include "op.h"
#include "data.h"
#include "list.h"
#include "vector.h"

char name_null[] = "null";
char name_int[] = "int";
char name_default[] = "default";
char name_float[] = "float";
char name_string[] = "string";
char name_char[] = "char";
char name_vertex[] = "vertex";
char name_face[] = "face";
char name_lst[] = "lst";
char name_vlst[] = "vlst";
char name_pointer[] = "pointer";
char name_mesh[] = "mesh";
char name_object[] = "object";
char name_selector[] = "selector";
char name_uint[] = "uint";
char name_camera[] = "camera";
char name_trigger[] = "trigger";
char name_operator[] = "operator";
char name_vector[] = "vector";
char name_viewport[] = "viewport";

char name_brick[] = "brick";
char name_block[] = "block";
char name_rhizome[] = "rhizome";
char name_graph[] = "graph";
char name_set[] = "set";

void cls_data_init(t_data *data)
{
}

t_data_cls cls_data=
{
	.cls_type="data",
	.init=cls_data_init,
};

char *data_name_get(t_data_type type)
{
	switch(type)
	{
		case dt_null: return name_null; break;
		case dt_int: return name_int; break;
		case dt_uint: return name_uint; break;
		case dt_float: return name_float; break; 
		case dt_char: return name_char; break;
		case dt_string: return name_string; break;
		case dt_pointer: return name_pointer; break;
		case dt_vertex: return name_vertex; break;
		case dt_face: return name_face; break;
		case dt_mesh: return name_mesh; break;
		case dt_lst: return name_lst; break;
		case dt_vlst: return name_vlst; break;
		case dt_camera: return name_camera; break;
		case dt_object: return name_object; break;
		case dt_selector: return name_selector; break;
		case dt_trigger: return name_trigger; break;
		case dt_operator: return name_operator; break;
		case dt_vector: return name_vector; break;
		case dt_viewport: return name_viewport; break;
		case dt_brick: return name_brick; break;
		case dt_block: return name_block; break;
		case dt_set: return name_set; break;
		case dt_rhizome: return name_rhizome; break;
		case dt_graph:  return name_graph; break;
		default: return name_default; break;
	}
}

void *data_add(t_data_type type,void *data)
{
	t_context *C=ctx_get();

	int *_int;
	unsigned int *_uint;
	float *_float;
	char *_char;
	char *_string=NULL;
	t_vlst *_vlst;
	t_vector *_vector;
	t_node *node;

	size_t size;


	switch(type)
	{
		case dt_vector:

			node = vector_add("vector");
			_vector = node->data;
			return _vector;

			break;
			
		case dt_int:

			_int = (int *)malloc(sizeof(int));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"int","int_data",sizeof(int),_int);
			}


			if(data) *_int=*((int*)data);
			else  *_int=0; 

			return _int;

			break;

		case dt_uint:

			_uint = (unsigned int *)malloc(sizeof(unsigned int));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene, "uint", "uint_data", sizeof(unsigned int), _uint);
			}


			if(data) *_uint = *((unsigned int*) data);
			else  *_uint = 0; 

			return _uint;

			break;

		case dt_char:
			
			_char = (char *)malloc(sizeof(char));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"char","char_data",sizeof(char),_char);
			}

			if(data) *_char=*((char*)data);
			else  *_char=0; 

			return _char;

			break;

			

		case dt_float:

			_float = (float *)malloc(sizeof(float));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"float","float_data",sizeof(float),_float);
			}

			if(data) *_float=*((float*)data);
			else *_float=0;

			return _float;

			break;

		case dt_string:

			if(data)
			{
				size=sizeof(char)*(strlen(data)+1);
				_string = (char *)malloc(size);
				strcpy(_string,data);

				if(C->scene->store)
				{
					scene_add_data_var(C->scene,"string","string_data",size,_string);
				}
			}

			return _string;

		case dt_mesh:
			printf("[MESH]\n");
			break;

		case dt_vertex:
			printf("[VERTEX]\n");
			break;

		case dt_face:
			printf("[FACE]\n");
			break;

		case dt_camera:
			printf("[CAMERA]\n");
			break;

		case dt_lst:
			printf("[LST]\n");
			break;

		case dt_object:
			printf("[OBJECT]\n");
			break;

		case dt_vlst:

			//XXX!!!
			if(data)
			{
				_vlst=(t_vlst *)data;

				return vlst_make("vlst",_vlst->type,_vlst->length,_vlst->count);
			}
			else
			{
				return NULL;
			}
			break;

		default:
			printf("[ERROR data_add] Unknown data type:%s\n",data_name_get(type));
			return NULL;
			break;
	}

	return NULL;
}

void *data_copy(t_data_type type,void *data)
{
	t_context *C=ctx_get();

	float *_float;
	char *_char;
	char *_string=NULL;
	t_vector *_vector;
	t_node *node;

	size_t size;

	void *data_new = data_add(type,data);

	t_vlst *vlst_new;
	t_vlst *vlst_old;

	int *_int_new;
	unsigned int *_uint_new;

	switch(type)
	{
		case dt_vector:

			node = vector_add("vector");
			_vector = node->data;
			return _vector;

			break;
			
		case dt_int:

			_int_new = (int *)data_new;

			return _int_new;

			break;

		case dt_uint:

			_uint_new = (unsigned int *)data_new;

			return _uint_new;

			break;

		case dt_char:
			
			_char = (char *)malloc(sizeof(char));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"char","char_data",sizeof(char),_char);
			}

			if(data) *_char=*((char*)data);
			else  *_char=0; 

			return _char;

			break;

			

		case dt_float:

			_float = (float *)malloc(sizeof(float));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"float","float_data",sizeof(float),_float);
			}

			if(data) *_float=*((float*)data);
			else *_float=0;

			return _float;

			break;

		case dt_string:

			if(data)
			{
				size=sizeof(char)*(strlen(data)+1);
				_string = (char *)malloc(size);
				strcpy(_string,data);

				if(C->scene->store)
				{
					scene_add_data_var(C->scene,"string","string_data",size,_string);
				}
			}

			return _string;

		case dt_mesh:
			printf("[MESH]\n");
			break;

		case dt_vertex:
			printf("[VERTEX]\n");
			break;

		case dt_face:
			printf("[FACE]\n");
			break;

		case dt_camera:
			printf("[CAMERA]\n");
			break;

		case dt_lst:
			printf("[LST]\n");
			break;

		case dt_object:
			printf("[OBJECT]\n");
			break;

		case dt_vlst:

			vlst_new = (t_vlst *)data_new;
			vlst_old = (t_vlst *)data;
			vlst_copy(vlst_new,vlst_old);
			//XXX
			vlst_new->is_linked = 0;

			return vlst_new;

			break;

		default:
			printf("[ERROR data_add] Unknown data type:%s\n",data_name_get(type));
			return NULL;
			break;
	}

	return NULL;
}

void data_init(t_data_type type,void *data_dst, void *data_src)
{
	int *_int;
	unsigned int *_uint;
	float *_float;
	t_vlst *_vlst;
	char *_char;

	switch(type)
	{
		case dt_int:

			_int = (int*) data_dst;
			*_int = 0; 

			break;

		case dt_uint:

			_uint = (unsigned int*) data_dst;
			*_uint = 0; 

			break;

		case dt_float:

			_float = (float*) data_dst;
			*_float = 0;

			break;

		case dt_string:

			printf("XXX\n");

			break;

		case dt_char:

			_char = (char *) data_dst;
			*_char = 0;

			break;

		case dt_mesh:
			printf("[MESH]\n");
			break;

		case dt_vertex:
			printf("[VERTEX]\n");
			break;

		case dt_face:
			printf("[FACE]\n");
			break;

		case dt_vlst:

			//XXX!!!
			if(data_dst)
			{
				_vlst = (t_vlst *) data_dst;
				vlst_init(_vlst);
			}
			break;

		case dt_lst:

			printf("[LST]\n");

			break;

		case dt_camera:

			printf("[CAMERA]\n");

			break;

		case dt_vector:

			if(data_src)
			{
				t_vector *vector_src = (t_vector *) data_src;
				t_vector *vector_dst = (t_vector *) data_dst;

				vector_dst->length = vector_src->length;
				vector_dst->type = vector_src->type;
			}

			break;

		default:

			printf("[ERROR data_init] Unknown data type %s\n",data_name_get(type));

			break;
	}

}

t_data *data_rebind(t_scene *sc,void *ptr)
{
	return ptr;
}

t_data *data_new(const char *name)
{
	t_data *data = (t_data *)malloc(sizeof(t_data));
	id_init(&data->id, name);
	bzero(data->type,_NAME_);
	bzero(data->target,_NAME_);
	data->size=0;
	data->pointer=NULL;
	data->ref=NULL;
	data->id_node=0;

	return data;
}

void data_free(t_data *data)
{
	free(data);
}

// DATUM

void datum_free(t_datum *datum)
{
	free(datum->data);
	free(datum);
}

t_datum *datum_new(t_data_type type, int count, void *data)
{
	t_datum *datum = (t_datum *) malloc(sizeof(t_datum));
	datum->type = type;
	datum->count = count;

	int i;
	switch(type)
	{
		case(dt_int):
			datum->data = (int *)malloc(sizeof(int) * count);
			if(data)
			{
				for(i=0;i<count;i++)
				{
					cprf_int(datum->data, data, i);
				}
			}	
			break;

		case(dt_bool):
			datum->data = (int *)malloc(sizeof(int) * count);
			if(data)
			{
				for(i=0;i<count;i++)
				{
					cprf_int(datum->data, data, i);
				}
			}	
			break;

		case(dt_float):
			datum->data = (int *)malloc(sizeof(float) * count);
			if(data)
			{
				for(i=0;i<count;i++)
				{
					cprf_int(datum->data, data, i);
				}
			}	
			break;
		default:
			printf("[ERROR datum_new] Unknown data type %s\n",data_name_get(type));
			break;
	}

	return datum;
}



