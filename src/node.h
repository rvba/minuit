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

struct Scene;
struct Lst;
struct Node;


// ID

/*
block plug camera data engine light vlst texture
object screen txt image link lst
*/

// NODE CLASS

typedef struct NodeClass
{
	t_node_type type;
	int size;
	struct Lst *lst;

	int  (* make)(struct Node *node);
	void (* build)(struct Node *node,const char *name);
	void (* link)(struct Node *node);
	void (* del)(struct Node *node);
	void (* init)(struct Node *node);
	void (* free)(struct Scene *sc,struct Node *node);
	void *(* get_ref)(struct Node *node, const char *ref);

}t_node_class;

// NODE

typedef struct Node
{
	int id;
	int id_old;
	int id_chunk;
	int id_chunk_self;
	void *id_ptr;
	void *id_ptr_old;
	int users;

	t_node_type type;
	struct NodeClass *cls;

	void *data;  
}t_node;


// NODE.C

extern char node_name_null[];
extern char node_name_brick[];
extern char node_name_mesh[];
extern char node_name_block[];
extern char node_name_light[];
extern char node_name_object[];
extern char node_name_screen[];
extern char node_name_file[];
extern char node_name_image[];
extern char node_name_material[];
extern char node_name_list[];
extern char node_name_link[];
extern char node_name_data[];
extern char node_name_texture[];
extern char node_name_var[];
extern char node_name_option[];
extern char node_name_vlst[];
extern char node_name_camera[];
extern char node_name_dict[];
extern char node_name_symbol[];
extern char node_name_vector[];
extern char node_name_set[];
extern char node_name_binding[];
extern char node_name_geo[];
extern char node_name_geo_point[];
extern char node_name_geo_edge[];
extern char node_name_geo_array[];

int 		node_equal(t_node *m, t_node *n);
t_node*		node_load(void *ptr,const char *type);
void		node_init(t_node *node,t_node_type type);
t_node *	node_new(t_node_type type);
t_node *	node_clone(t_node *src);
char *		node_name_get(t_node_type type);

void id_init(t_id *id, const char *name);

#endif

