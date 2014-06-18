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

#ifdef __cplusplus
extern "C" {
#endif

struct VLst;
struct Scene;

typedef struct Vector t_vector;
typedef struct Vector_Class t_vector_cls;

struct Vector
{
	t_id id;

	t_vector_cls *cls;

	t_data_type type;
	int length;

	void *pointer;
	struct VLst *vector; 
};

struct Vector_Class
{
	void (* init)(t_vector *vector);
};

void *vector_get_pointer(t_vector *vector, int indice);

void vector_copy_pointer(t_vector *dst, t_vector *src);
int vector_is_different(t_vector *dst, t_vector *src);
void * vector_new(const char *name);
t_node *vector_add(const char *name);
void vector_free(t_vector *vector);
void vector_delete( t_vector *vector);

void vector_op_copy(t_vector *dst, t_vector *src);
void vector_op_add(t_vector *dst,t_vector *src);
void vector_show(t_vector *vector);

void vector_rebind( struct Scene *sc,void *ptr);

#ifdef __cplusplus
}
#endif

#endif
