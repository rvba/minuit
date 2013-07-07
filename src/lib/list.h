/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _LIST_H
#define _LIST_H

#include "data.h"

struct Scene;
struct Node;

// LINK

typedef struct Link
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	int order;		// Used by rhizome sorting

	struct Link *next;
	struct Link *prev;

	void *data;

}t_link;

// LST

typedef struct Lst
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	t_link *first;
	t_link *last;
	t_link *current;

	int count;

}t_lst;

typedef struct VLst
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	// generic

	int count;	// number of block
	int length;	// number of element in block 
	int size;	// size of element (int,float,...)

	int count_new;
	int need_update;
	int is_linked;
	int has_limit_high;
	int has_limit_low;
	int limit_high;
	int limit_low;

	struct VLst *link;

	t_data_type type;

	void *data;
}t_vlst;

void 		lst_remove_doubles(t_lst *lst);

void 		vlst_update_data(t_vlst *vlst,t_vlst *caller);
void		vlst_free(t_vlst *vlst);
void 		vlst_init(t_vlst *vlst);
t_vlst *	vlst_new(const char *name);
t_vlst *	vlst_make(const char *name,t_data_type type,int length, int count);

void *		vlst_get_ref(t_vlst *vlst, const char *ref);
void *		vlst_get_pointer(t_vlst *vlst, int indice);
void 		vlst_set_data(t_vlst *vlst, void *data, int indice);

t_vlst *	vlst_rebind(struct Scene *sc,void *ptr);
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
t_vlst *vlst_clone(t_vlst *vlst);


// LST


int 		lst_sort_bubble(t_lst *lst);
int 		lst_sort_quick(t_lst *lst);

void 		lst_link_insert(t_lst *lst, t_link *link, t_link *prev);

 
t_link * 	list_add(t_lst *lst,void *data);
t_link * 	list_add_global(t_lst *lst,void *data);
t_lst *		lst_rebind(struct Scene *sc,void *ptr);
t_link *	link_rebind(struct Scene *sc,void *ptr);

int 		lst_remove_node(t_lst *lst,struct Node *node);
void 		lst_remove_all(t_lst *lst);
void *		lst_get(t_lst *lst,char *name);

struct Node *	lst_find_node_by_name(t_lst *lst,const char *name);
int 		lst_remove_by_id(t_lst *lst,int id);

void 		list_remove_by_ptr(t_lst *lst,void *ptr);
void 		list_free(t_lst *lst);
void 		_link_free(t_link *link);


void 		list_remove_by_name(t_lst *lst, const char *name);
void 		lst_show_generic(t_lst *lst);

void 		list_cleanup(t_lst *lst);
void 		list_show(t_lst *lst);

struct Node *	lst_get_node(t_lst *lst,int id);

struct Node *	lst_find_node(t_lst *lst,const char *name);

// LST

t_link *	lst_link_find_by_name(t_lst *lst, const char *name);
void 		lst_link_delete_by_name(t_lst *lst, const char *name);

void 		lst_show(t_lst *lst);
t_link *	lst_add(t_lst *lst,void *data,const char *name);
void 		lst_add_lst(t_lst *dst, t_lst *src);
void		lst_link_remove(t_lst *lst,t_link *link);
void 		lst_delete_all(t_lst *lst);
void 		lst_link_delete(t_lst *lst,t_link *link);
t_lst *		lst_copy(t_lst *lst);
t_lst *		lst_new(const char *name);
void 		lst_free(t_lst *lst);
void 		lst_cleanup(t_lst *lst);
void 		lst_push_back(t_lst *lst,t_link *link);

void 		lst_remove_by_ptr(t_lst *lst,void *ptr);
void 		list_remove_by_id(t_lst *lst, int id);
void 		link_free(t_link *link);

t_link *	link_new(const char *name);
void 		link_free(t_link *link);

void _list_free(t_lst *lst, t_data_type type);
t_lst *lst_clone(t_lst *lst, t_data_type type);

#endif
