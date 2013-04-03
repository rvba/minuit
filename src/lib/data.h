/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __DATA_H
#define __DATA_H

#include "util.h"

typedef struct Data t_data;
typedef struct Data_Class t_data_cls;

struct Data_Class
{
	char cls_type[_NAME_];
	void (* init)(t_data *data);
};

struct Data
{
	int id; 
	int id_chunk;
	short users;
	char name[_NAME_];
	t_data_cls *cls;

	char type[_NAME_];
	char target[_NAME_];

	int size;

	void *pointer;
	void *ref;

	int id_node;
};


char *name_null;
char *name_int;
char *name_default;

char *name_float;
char *name_string;
char *name_char;
char *name_vertex;
char *name_face;
char *name_lst;
char *name_vlst;
char *name_pointer;
char *name_mesh;
char *name_object;
char *name_selector;
char *name_uint;
char *name_camera;
char *name_trigger;
char *name_operator;
char *name_vector;

char *data_name_get(t_data_type);


void		print_type(t_data_type type);
void 		data_init(t_data_type type,void *data);
void *		data_add(t_data_type type,void *data);
void *		data_copy(t_data_type type,void *data);
t_data *	data_new(const char *name);
void		data_free(t_data *data);


#endif
