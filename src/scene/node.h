/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __NODE_H
#define __NODE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct Lst;
struct Node;

typedef struct NodeClass
{
	t_data_type type;
	int size;
	struct Lst *lst;

	void (* build)(struct Node *node,const char *name);
	void (* link)(struct Node *node);
	void (* del)( void *data);
	void (* init)(struct Node *node);
	void (* free)(struct Scene *sc,struct Node *node);
	void *(* get_ref)(struct Node *node, const char *ref);
	void (* rebind)(struct Scene *scene, void *ptr);

}t_node_class;

typedef struct Node
{
	int id;
	int id_old;
	int id_chunk;
	int id_chunk_self;
	void *id_ptr;
	void *id_ptr_old;
	int users;
	int store;
	int garbage;
	int size;

	t_data_type type;
	struct NodeClass *cls;

	void *data;  
}t_node;


int 		node_equal(t_node *m, t_node *n);
t_node*		node_load(void *ptr,const char *type);
void		node_init(t_node *node,t_data_type type);
t_node *	node_new(t_data_type type);
t_node *	node_clone(t_node *src);

void id_init	(t_id *id, const char *name);

void var_rebind( struct Scene *scene, void *ptr);

#ifdef __cplusplus
}
#endif

#endif

