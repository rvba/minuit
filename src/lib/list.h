/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __LIST_H
#define __LIST_H

#include "data.h"

struct Context;
struct Scene;
struct Node;
struct Lst;
struct Link;

void 		lst_remove_doubles(struct Lst *lst);

 
struct Link * 	list_add_data( struct Lst *lst, void *data);
struct Link * 	list_add_global(struct Lst *lst,void *data);
struct Lst *	list_rebind(struct Scene *sc,void *ptr);
struct Link *	link_rebind(struct Scene *sc,void *ptr);
int 		list_remove_node(struct Lst *lst,struct Node *node);
struct Node *	list_find_node_by_name(struct Lst *lst,const char *name);
void 		list_remove_by_ptr(struct Lst *lst,void *ptr);
void 		list_free(struct Lst *lst);
void 		_link_free(struct Link *link);
void 		list_remove_by_name(struct Lst *lst, const char *name);
void 		lst_show_generic(struct Lst *lst);
void 		list_cleanup(struct Lst *lst);
void 		list_show(struct Lst *lst);
struct Node *	list_node_exists(struct Lst *lst,int id);
void 		list_free_data(struct Lst *lst, t_data_type type);
struct Lst *	list_clone(struct Lst *lst, t_data_type type);
void 		list_delete( struct Lst *lst);
struct Lst *	list_make( t_data_type type,  const char *name);
void 		cls_list_delete( void *data);

#endif
