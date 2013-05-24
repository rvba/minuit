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

#define DATA_NAME 64

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

extern char name_null[];
extern char name_int[];
extern char name_default[];
extern char name_float[];
extern char name_string[];
extern char name_char[];
extern char name_vertex[];
extern char name_face[];
extern char name_lst[];
extern char name_vlst[];
extern char name_pointer[];
extern char name_mesh[];
extern char name_object[];
extern char name_selector[];
extern char name_uint[];
extern char name_camera[];
extern char name_trigger[];
extern char name_operator[];
extern char name_vector[];
extern char name_viewport[];

char *		data_name_get(t_data_type);
void		print_type(t_data_type type);
void		 data_init(t_data_type type,void *data_dst, void *data_src);
void *		data_add(t_data_type type,void *data);
void *		data_copy(t_data_type type,void *data);
t_data *	data_rebind(t_scene *sc,void *ptr);
t_data *	data_new(const char *name);
void		data_free(t_data *data);


#endif
