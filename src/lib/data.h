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

#define DATA_NAME 64

#include "common.h"

struct Scene;
struct Data;

typedef enum Data_Type
{
	dt_null,
	dt_bool,
	dt_int,
	dt_uint,
	dt_float,
	dt_char,
	dt_string,
	dt_pointer,
	dt_vertex,
	dt_face,
	dt_mesh,
	dt_lst,
	dt_vlst,
	dt_camera,
	dt_object,
	dt_selector,
	dt_trigger,
	dt_operator,
	dt_vector,
	dt_viewport,
	dt_binding,
	dt_brick,
	dt_block,
	dt_rhizome,
	dt_graph,
	dt_set,
	dt_geo,
	dt_geo_point,

}t_data_type;

typedef struct Datum
{
	t_data_type type;
	int count;
	void *data;
}t_datum;

typedef struct Data_Class
{
	char cls_type[_NAME_];
	void (* init)(struct Data *data);

}t_data_cls;

typedef struct Data
{
	t_id id;
	t_data_cls *cls;

	char type[_NAME_];
	char target[_NAME_];

	int size;

	void *pointer;
	void *ref;

	int id_node;
}t_data;

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
extern char name_rhizome[];
extern char name_brick[];
extern char name_block[];
extern char name_set[];
extern char name_geo_point[];
extern char name_geo[];

char *		data_name_get(t_data_type);
void		print_type(t_data_type type);
void		 data_init(t_data_type type,void *data_dst, void *data_src);
void *		data_add(t_data_type type,void *data);
void *		data_copy(t_data_type type,void *data);
t_data *	data_rebind(struct Scene *sc,void *ptr);
t_data *	data_new(const char *name);
void		data_free(t_data *data);

t_datum *datum_new(t_data_type type, int count, void *data);
void datum_free(t_datum *datum);

#endif
