/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __BLOCK_H
#define __BLOCK_H

#define BRICK_MIN_WIDTH 130

#include "common.h"

struct Dict;
struct Scene;
struct Brick;
struct Node;
struct Lst;

typedef struct Block t_block;
typedef struct Block_Class t_block_class;
typedef struct Block_State t_block_state;



typedef enum Type_Parent
{
	t_parent_child,
	t_child_parent
}t_parent;

typedef enum Type_Operation 
{
	t_op_add,
	t_op_mult

}t_operation;

/***		BLOCK		***/

struct Block_Class
{
	char cls_type[_NAME_];
	char type[_NAME_];
	void (* make)(t_block *self);
	void (* link)(t_block *self,struct Node *target);
	void (* draw)(t_block *self);
	void (* update)(t_block *self);
	void (* init)(t_block *self);
};

struct Block_State
{	
	int is_root:1;
	int is_show:1;	
	int draw_outline:1;
	int draw_plugs:1;
	int is_mouse_over:1;
	int update_geometry:1;
};

struct Block
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_block_class *cls;

	int idcol[3];
	char type[_NAME_];

	float pos[3];				// block position in space

	float width;				// block width
	float height;				// block height

	t_block_state state;			// block state

	int tot_bricks; 			// drawing plugs
	int graph_order;			// unset > -1

	struct Lst *bricks;
	struct Brick *submenu;
	struct Brick *selected;			// submenu
};



// BLOCK

struct Brick *	block_brick_get_by_order(t_block *block, int order);
void 		block_set_graph_order(t_block *block, int order);
int 		block_is_connected(const char *gate, t_block *block);
struct Lst 		*block_get_connections(const char *gate,t_block *block);
t_block *	block_copy(t_block *block);
t_block *	block_clone(t_block *block);
struct Brick*	block_brick_get(t_block *block,const char *name);
void 		block_brick_add(t_block *block,struct Node *node_brick);
struct Node*		block_create(const char *name);
struct Node*		block_make(const char *name,const char *type);
void 		block_free(t_block *block);
t_block *	block_new(const char *name);
t_block *	block_rebind(struct Scene *sc,void *ptr);


// BLOCK DRAW

void 		block_draw(t_block *block);
void		cls_block_draw_generic(t_block *self);
void		cls_block_draw_menu(t_block *self);
void		cls_block_draw_ref(t_block *self);
void		cls_block_draw_block(t_block *self);

// BLOCK_UDPATE

void		ctx_block_unstore(void);
void 		context_set_block(void);
void 		cls_block_update(t_block *block);
void		cls_block_generic_update(t_block *self);
void		cls_block_ref_update(t_block *self);
void		cls_block_menu_update(t_block *self);
void		cls_block_block_update(t_block *self);
void		block_unstore(t_block *block);

void 		set_for_loop(t_block *block ,int state);

#endif




