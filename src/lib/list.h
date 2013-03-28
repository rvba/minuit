/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "minuit.h"

#ifndef _LIST_H
#define _LIST_H


enum VLst_Type
{
	_0,
	_1f,
	_2f,
	_3f,
	_4f,
	_1i,
	_2i,
	_3i,
	_4i,
	_1ui,
	_2ui,
	_3ui,
	_4ui,
	_1c,
	_2c,
	_3c,
	_4c,
	_1uc,
	_2uc,
	_3uc,
	_4uc,

};

struct VLst
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	// generic

	t_vlst_type type;

	int count;	// number of block
	int length;	// number of element in block 
	size_t size;	// size of element (int,float,...)

	int count_new;
	int need_update;
	int is_linked;

	t_vlst *link;
	
	t_data_type data_type;

	void *data;
};

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
};

struct Vector_Class
{
	void (* init)(t_vector *vector);
};


//void 		vlst_update_data(t_vlst *vlst);
void 		vlst_update_data(t_vlst *vlst,t_vlst *caller);
void		vlst_free(t_vlst *vlst);
void 		vlst_init(t_vlst *vlst);
t_vlst *	vlst_new(const char *name);
t_vlst *	vlst_make(const char *name,t_vlst_type type,int count);
t_vlst *	vlst_rebind(t_scene *sc,void *ptr);
void 		vlst_normal_3f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_normal_4f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void		 vlst_normal_4f_direct(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_fill_3f(t_vlst *vlst,float x,float y,float z);
void 		vlst_fill_4f(t_vlst *vlst,float x,float y,float z,float w);
void 		vlst_color_3f(t_vlst *vlst,int *color);
void 		vlst_color_4f(t_vlst *vlst,int *color);
void 		vlst_show(t_vlst *vlst);
void 		vlst_vertex(const char *type,t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		_do_face(void *src,void *dst,int count,int size);
void 		vlst_build(t_vlst *vlst,void *ptr);
void 		vlst_add_data(t_vlst *vlst,void *ptr);
void 		vlst_copy(t_vlst *dst,t_vlst *src);
void 		vlst_normal_4f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_add_vlst(t_vlst *vlst_dst,t_vlst *vlst_src);

void 		vlst_add_number(t_vlst *vlst,t_data_type type,void *data);

t_vlst *	vlst_duplicate(t_vlst *vlst);
void 		vlst_set(t_vlst *vlst,int i);

 
t_link * 	list_add(t_lst *lst,void *data);
t_link * 	list_add_global(t_lst *lst,void *data);
t_lst *		lst_rebind(t_scene *sc,void *ptr);
t_link *	link_rebind(t_scene *sc,void *ptr);

t_node *	lst_get_node(t_lst *lst,int id);
int 		lst_remove_node(t_lst *lst,t_node *node);
void 		lst_remove_all(t_lst *lst);
void *		lst_get(t_lst *lst,char *name);

t_node *	lst_find_node_by_name(t_lst *lst,const char *name);
//void *		lst_get_by_id(t_lst *lst,int id);
int 		lst_remove_by_id(t_lst *lst,int id);

void list_remove_by_ptr(t_lst *lst,void *ptr);
void list_free(t_lst *lst);
void _link_free(t_link *link);

t_vector *vector_new(const char *name);
t_node *vector_add(const char *name);
void vector_free(t_vector *vector);

void lst_show_generic(t_lst *lst);

void list_cleanup(t_lst *lst);

#endif
