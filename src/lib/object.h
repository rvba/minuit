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

#include "common.h"

#define OBJECT_SHADER_ON 1
#define OBJECT_SHADER_OFF 2

#ifdef __cplusplus
extern "C" {
#endif

struct Node;
struct Mesh;
struct Block;
struct Object;
struct Lst;
struct Scene;
struct Context;

// OBJECT CLASS

typedef struct Object_Class
{
	char cls_type[_NAME_];
	char type[_NAME_];
	struct Node *(* get_mesh)(struct Node *node);
	void (* link)(struct Object *self,struct Node *target);
	void (* show)(struct Object *self);
	void (* draw)(struct Object *self);
	void (* init)(struct Object *object);

}t_object_class;

// OBJECT

typedef struct Object
{
	t_id id;
	t_object_class *cls;

	int idcol[3];
	char type[_NAME_];

	float loc[3];
	float rot[3];
	float size[3];

	int is_selected;
	int is_edit_mode;
	int hover;

	void *data; 		// struct (data)

	struct Mesh *mesh;		// mesh
	struct Lst *blocks;		// buttons
	struct Block *ref;

	void (* action)(struct Node *node);
	void (* update)(struct Node *node);
	void (* draw)(struct Node *node);
	void (* shader)( struct Object *object, int state);

}t_object;

// METHODS
void *		object_get_ref(t_object *object, const char *ref);

void 		object_mesh_add(struct Node *node,struct Node *mesh);
void 		object_draw_add(struct Node *node,void (* func)(struct Node *node));
void 		object_method_add(struct Node *node,const char method[],void (*func)(struct Node *n));
void 		object_data_add(struct Node *node,void *ptr);
void 		object_block_add( struct Object *object, struct Block *block);


struct Node*	op_object_mesh_get(struct Node *node);

void object_draw_blocks( struct Context *C,  t_object *object);

void 		cls_object_draw_camera(t_object *object);
void 		cls_object_draw_mesh(t_object *object);
void 		cls_object_draw_light(t_object *object);
void 		cls_object_draw_point(t_object *object);

t_object *	object_clone(t_object *object);
t_object *	object_rebind(struct Scene *sc,void *ptr);
void 		object_build(t_object *object,const char *type);
struct Node *	object_make(const char *type,const char *name);
struct Node *	object_add(const char *type,const char *name);
t_object*	object_new(const char *name);

void 		_object_free(t_object *object);
void 		object_free(t_object *object);

#ifdef __cplusplus
}
#endif


#endif

