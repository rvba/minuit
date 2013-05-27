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

typedef struct Node t_node;
typedef struct NodeClass t_node_class;
typedef enum  Node_Type t_node_type;

typedef struct Generic t_generic;
typedef struct Generir_Class t_generic_cls;


enum Node_Type
{
	nt_null,
	nt_mesh,
	nt_block,
	nt_brick,
	nt_light,
	nt_object,
	nt_screen,
	nt_file,
	nt_image,
	nt_material,
	nt_list,
	nt_link,
	nt_data,
	nt_texture,
	nt_var,
	nt_option,
	nt_vlst,
	nt_camera,
	nt_dict,
	nt_symbol,
	nt_vector,
	nt_viewport,
	nt_set,
	nt_binding,
};


// GENERIC

/*
block plug camera data engine light vlst texture
object screen txt image link lst
*/

struct Generic_Class
{
	char cls_type[_NAME_];
};

struct Generic
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_generic_cls *cls;
};

// NODE CLASS

struct NodeClass
{
	t_node_type type;
	int size;
	struct Lst *lst;

	int  (* make)(t_node *node);
	void (* build)(t_node *node,const char *name);
	void (* link)(t_node *node);
	void (* del)(t_node *node);
	void (* init)(t_node *node);
	void (* free)(struct Scene *sc,t_node *node);

	void (* set_state_selected)(t_node *node,int state);
	int (* is_mouse_over)(t_node *node);
	void *(* get_ref)(t_node *node, const char *ref);
};

// NODE

struct Node
{
	int id;
	int id_old;
	int id_chunk;
	int id_chunk_self;
	void *id_ptr;
	void *id_ptr_old;
	int users;

	t_node_type type;
	t_node_class *cls;

	void *data;  
};


// NODE.C

extern char node_name_null[];
extern char node_name_generic[];
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

t_node*		node_load(void *ptr,const char *type);
void		node_init(t_node *node,t_node_type type);
t_node *	node_new(t_node_type type);
t_node *	node_clone(t_node *src);

int 		dlink(const char *type,void *ptr);

char *node_name_get(t_node_type type);

#endif

