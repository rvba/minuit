/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "op.h"
#include "data.h"
#include "list.h"
#include "vlst.h"
#include "vector.h"
#include "geometry.h"
#include "ui.h"
#include "memory.h"

void cls_data_init(t_data *data)
{
}

t_data_cls cls_data=
{
	.cls_type="data",
	.init=cls_data_init,
};

const char *data_name_get(t_data_type type)
{
	switch(type)
	{
		case dt_null: return "null"; break;
		case dt_int: return "int"; break;
		case dt_uint: return "uint"; break;
		case dt_float: return "float"; break; 
		case dt_char: return "char"; break;
		case dt_uchar: return "uchar"; break;
		case dt_string: return "string"; break;
		case dt_pointer: return "pointer"; break;
		case dt_vertex: return "vertex"; break;
		case dt_face: return "face"; break;
		case dt_mesh: return "mesh"; break;
		case dt_lst: return "lst"; break;
		case dt_vlst: return "vlst"; break;
		case dt_camera: return "camera"; break;
		case dt_object: return "object"; break;
		case dt_selector: return "selector"; break;
		case dt_trigger: return "trigger"; break;
		case dt_operator: return "operator"; break;
		case dt_vector: return "vector"; break;
		case dt_viewport: return "viewport"; break;
		case dt_brick: return "brick"; break;
		case dt_block: return "block"; break;
		case dt_set: return "set"; break;
		case dt_rhizome: return "rhizome"; break;
		case dt_graph:  return "graph"; break;
		case dt_geo_point:  return "geo_point"; break;
		case dt_geo_edge:  return "geo_edge"; break;
		case dt_geo_array:  return "geo_array"; break;
		case dt_geo:  return "geo"; break;
		case dt_dict: return "dict"; break;
		case dt_symbol: return "symbol"; break;
		case dt_var: return "var"; break;
		case dt_list: return "list"; break;
		case dt_link: return "link"; break;
		case dt_data: return "data"; break;
		case dt_screen: return "screen"; break;
		case dt_option: return "option"; break;
		case dt_binding: return "binding"; break;
		case dt_material: return "material"; break;
		case dt_light: return "light"; break;
		case dt_datum: return "datum"; break;
		case dt_undefined: return "undefined"; break;
		case dt_texture: return "texture"; break;
		case dt_image: return "image"; break;
		case dt_module: return "module"; break;
		case dt_point: return "point"; break;
		default: printf("data_name_get type not implemented %d\n", type);return "-"; break;
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
	//t_geo_point *_point;

	size_t size;


	switch(type)
	{

		case dt_geo_point:

			return data;
			break;

		case dt_geo_edge:

			return data;
			break;

		case dt_geo_array:

			return data;
			break;

		case dt_geo:

			return data;
			break;

		case dt_vector:

			node = vector_add("vector");
			_vector = node->data;
			return _vector;

			break;
			
		case dt_int:

			_int = (int *)mem_malloc(sizeof(int));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"int","int_data",sizeof(int),_int);
			}


			if(data) *_int=*((int*)data);
			else  *_int=0; 

			return _int;

			break;

		case dt_uint:

			_uint = (unsigned int *)mem_malloc(sizeof(unsigned int));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene, "uint", "uint_data", sizeof(unsigned int), _uint);
			}


			if(data) *_uint = *((unsigned int*) data);
			else  *_uint = 0; 

			return _uint;

			break;

		case dt_char:
			
			_char = (char *)mem_malloc(sizeof(char));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"char","char_data",sizeof(char),_char);
			}

			if(data) *_char=*((char*)data);
			else  *_char=0; 

			return _char;

			break;

			

		case dt_float:

			_float = (float *)mem_malloc(sizeof(float));

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
				_string = (char *)mem_malloc(size);
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

				return vlst_make("vlst",_vlst->type,_vlst->length,_vlst->count, data);
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
			
			_char = (char *)mem_malloc(sizeof(char));

			if(C->scene->store)
			{
				scene_add_data_var(C->scene,"char","char_data",sizeof(char),_char);
			}

			if(data) *_char=*((char*)data);
			else  *_char=0; 

			return _char;

			break;

			

		case dt_float:

			_float = (float *)mem_malloc(sizeof(float));

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
				_string = (char *)mem_malloc(size);
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
			vlst_copy_data(vlst_new,vlst_old);
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
	t_geo_point *_geo_point_src;
	t_geo_point *_geo_point_dst;

	t_geo_edge *_geo_edge_src;
	t_geo_edge *_geo_edge_dst;

	t_geo_array *_geo_array_src;
	t_geo_array *_geo_array_dst;

	t_geo *_geo_src;
	t_geo *_geo_dst;

	switch(type)
	{

		case dt_geo:
			_geo_dst = (t_geo *) data_dst;
			_geo_src = (t_geo *) data_src;

			*_geo_dst = *_geo_src;
			
			break;

		case dt_geo_array:
			_geo_array_dst = (t_geo_array *) data_dst;
			_geo_array_src = (t_geo_array *) data_src;

			*_geo_array_dst = *_geo_array_src;
			
			break;

		case dt_geo_point:
			_geo_point_dst = (t_geo_point *) data_dst;
			_geo_point_src = (t_geo_point *) data_src;

			*_geo_point_dst = *_geo_point_src;
			
			break;

		case dt_geo_edge:
			_geo_edge_dst = (t_geo_edge *) data_dst;
			_geo_edge_src = (t_geo_edge *) data_src;

			*_geo_edge_dst = *_geo_edge_src;
			
			break;

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

void data_rebind(t_scene *sc,void *ptr)
{
}

void *data_new(const char *name)
{
	t_data *data = (t_data *)mem_malloc(sizeof(t_data));
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
	mem_free( data, sizeof( t_data));
}


