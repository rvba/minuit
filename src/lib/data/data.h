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

#include "base.h"

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

const char *	data_name_get(t_data_type);
void		print_type(t_data_type type);
void		data_init(t_data_type type,void *data_dst, void *data_src);
void *		data_add(t_data_type type,void *data);
void *		data_copy(t_data_type type,void *data);
void 		data_rebind(struct Scene *sc,void *ptr);
void *		data_new(const char *name);
void		data_free(t_data *data);


#ifdef __cplusplus
}
#endif



#endif
