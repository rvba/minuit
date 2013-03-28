/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OBJECT_H
#define __OBJECT_H

#include "node.h"
#include "mesh.h"
#include "block.h"

typedef struct Object t_object;
typedef struct Object_Class t_object_class;

// OBJECT CLASS

struct Object_Class
{
	char cls_type[_NAME_];
	char type[_NAME_];
	t_node *(* get_mesh)(t_node *node);
	void (* link)(t_object *self,t_node *target);
	void (* show)(t_object *self);
	void (* draw)(t_object *self);
	void (* init)(t_object *object);
};

// OBJECT

struct Object
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_object_class *cls;

	int idcol[3];

	char type[_NAME_];


	float loc[3];
	float rot[3];
	float size[3];

	int is_selected;

	void *data; 		// struct (data)
	t_mesh *mesh;		// mesh
	t_lst *blocks;		// buttons
	t_block *ref;

	void (* action)(t_node *node);
	void (* update)(t_node *node);
	void (* draw)(t_node *node);
};

// METHODS

void 		object_mesh_add(t_node *node,t_node *mesh);
void 		object_draw_add(t_node *node,void (* func)(t_node *node));
void 		object_method_add(t_node *node,const char method[],void (*func)(t_node *n));
void 		object_data_add(t_node *node,void *ptr);
t_node*		op_object_mesh_get(t_node *node);


void 		cls_object_draw_camera(t_object *object);
void 		cls_object_draw_mesh(t_object *object);
void 		cls_object_draw_light(t_object *object);

t_object *	object_rebind(t_scene *sc,void *ptr);
void 		object_build(t_object *object,const char *type);
t_node *	object_make(const char *type,const char *name);
t_node *	object_add(const char *type,const char *name);
t_object*	object_new(const char *name);
void 		object_free(t_object *object);


#endif

