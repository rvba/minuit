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

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_NAME 64

struct Scene;
struct Data;


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
extern char name_geo_edge[];
extern char name_geo[];
extern char name_dict[];
extern char name_symbol[];
extern char name_var[];
extern char name_link[];
extern char name_data[];
extern char name_screen[];
extern char name_option[];
extern char name_binding[];
extern char name_material[];
extern char name_light[];
extern char name_point[];


char *		data_name_get(t_data_type);
void		print_type(t_data_type type);
void		 data_init(t_data_type type,void *data_dst, void *data_src);
void *		data_add(t_data_type type,void *data);
void *		data_copy(t_data_type type,void *data);
void 		data_rebind(struct Scene *sc,void *ptr);
void *		data_new(const char *name);
void		data_free(t_data *data);


#ifdef __cplusplus
}
#endif



#endif
