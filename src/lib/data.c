/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"


char *name_null = "null";
char *name_int = "int";
char *name_default = "default";

char *name_float = "float";
char *name_string = "string";
char *name_char = "char";
char *name_vertex = "vertex";
char *name_face = "face";
char *name_lst = "lst";
char *name_vlst = "vlst";
char *name_pointer = "pointer";
char *name_mesh = "mesh";
char *name_object = "object";
char *name_selector = "selector";
char *name_uint = "uint";
char *name_camera = "camera";
char *name_trigger = "trigger";
char *name_operator = "operator";
char *name_vector = "vector";
char *name_vewport = "viewport";


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
		default: return name_default; break;
	}
}

void *data_add(t_data_type type,void *data)
{
	t_context *C=ctx_get();

	int *_int;
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

				return vlst_make("vlst",_vlst->type,_vlst->count);
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

void data_init(t_data_type type,void *data)
{
	int *_int;
	float *_float;
	t_vlst *_vlst;
	char *_char;

	switch(type)
	{
		case dt_int:

			_int=(int*)data;
			*_int=0; 

			break;

		case dt_float:

			_float=(float*)data;
			*_float=0;

			break;

		case dt_string:

			printf("XXX\n");

			break;

		case dt_char:

			_char=(char *)data;
			*_char=0;

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
			if(data)
			{
			_vlst=(t_vlst *)data;
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

			//XXX
			//printf("[VECTOR]\n");

			break;

		default:

			printf("[ERROR data_init] Unknown data type %s\n",data_name_get(type));

			break;
	}

}

t_data *data_new(const char *name)
{
	t_data *data = (t_data *)malloc(sizeof(t_data));
	data->id=0;
	data->id_chunk=0;
	set_name(data->name,name);
	data->users=0;
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

