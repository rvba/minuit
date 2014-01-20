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

#include "common.h"

#define BRICK_MIN_WIDTH 130
#define BLOCK_STATE_DEFAULT 1
#define BLOCK_STATE_HOVER_
#define BLOCK_STATE_HOVER_MENU 2

#define BLOCK_SWAP( block, st) ((block->state) = &(st)) 

struct Dict;
struct Scene;
struct Brick;
struct Node;
struct Lst;
struct Plug;
struct Dot;
struct Event;

typedef struct Block t_block;
typedef struct Block_Class t_block_class;
typedef struct Block_State t_block_state;

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
	void (* init)(t_block *self);
	void (* dispatch)(t_block *self);
};

struct Block_State
{	
	int is_root:1;
	int draw_outline:1;
	int draw_plugs:1;
	int is_mouse_over:1;
	int update_geometry:1;
	int is_a_loop:1;
	int is_in_rhizome:1;
	int frame_based:1;
};

struct Block
{
	t_id id;
	t_block_class *cls;

	int idcol[3];
	char type[_NAME_];

	float pos[3];				// block position in space

	float width;				// block width
	float height;				// block height

	t_block_state block_state;			// block state

	int tot_bricks; 			// drawing plugs
	int rhizome_order;			// unset > -1
	int rhizome_pos;

	struct Lst *bricks;
	struct Brick *submenu;
	struct Brick *hover;			// submenu
	struct Brick *selected;			// submenu

	struct Rhizome *rhizome;
	struct Set *set;

	void (* state)( t_block *block, struct Event *e);
};



// BLOCK

void block_brick_set_order( t_block *block);
void 		block_dash_add(t_block *block, struct Dot *dot_x, struct Dot *dot_y);

struct Brick *	block_brick_get_by_order(t_block *block, int order);
struct Brick *	block_brick_get_by_position(t_block *block, int pos);
void 		block_set_rhizome_order(t_block *block, int order);
int 		block_is_connected(const char *gate, t_block *block);
t_block *	block_copy(t_block *block);
t_block *	block_clone(t_block *block);
t_block *	block_dupli(t_block *block);
struct Brick*	block_brick_get(t_block *block,const char *name);
void 		block_brick_add(t_block *block,struct Node *node_brick);

struct Node*		block_create(const char *name);
struct Node*		block_make(const char *name,const char *type);
void 		block_free(t_block *block);
t_block *	block_new(const char *name);
t_block *	block_rebind(struct Scene *sc,void *ptr);

void cls_block_dispatch( t_block *block);
void cls_block_dispatch( t_block *block);

void cls_block_state_block_default( t_block *block, struct Event *e);
void state_block_menu_default( t_block *block, struct Event *e);
void state_block_default( t_block *block, struct Event *e);




// BLOCK DRAW

void 		block_draw_outline(t_block *block);
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

void 		cls_block_generic_update(t_block *block);

void 		block_rhizome_split(struct Block *block_self, struct Plug *plug_self, struct Block *block_dst, struct Plug *plug_dst);
void 		block_rhizome_add(t_block *self, t_block *dst);

void 		block_set_loop_state(t_block *block, int state);
void 		block_exec(t_block *block);
void 		_block_free(t_block *block);
void 		block_get_center( t_block *block, float *v);
void 		block_get_pos_plug_out( t_block *block, struct Brick *brick, float *v);

#endif




