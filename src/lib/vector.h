/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __VECTOR_H
#define __VECTOR_H


struct Vector
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	t_vector_cls *cls;

	t_data_type type;
	int length;

	void *data;
	t_vlst *vlst;
};

struct Vector_Class
{
	void (* init)(t_vector *vector);
};

t_vector *vector_new(const char *name);
t_node *vector_add(const char *name);
void vector_free(t_vector *vector);

void vector_op_copy(t_vector *dst, t_vector *src);
void vector_op_add(t_vector *dst,t_vector *src);
void vector_show(t_vector *vector);

t_vector *vector_rebind(t_scene *sc,void *ptr);


#endif
