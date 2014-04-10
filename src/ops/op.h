/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OPS_H
#define __OPS_H

#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Context;
struct Node;
struct Block;
struct Brick;
struct Lst;

extern struct Block *menu_node;

// OP_DEBUG

void op_debug_all(struct Context *C);

extern int set_draw_plug;

// OP_ACTION

void 		op_switch_color(struct Context *C);
void		op_set_color(struct Context *C,int color);
int 		op_exit(void);
int 		op_quit(struct Node *node);
void 		op_export(struct Node *node);

// OP_C

void 		op_add_global(struct Context *C,struct Block *block);
void 		op_init(struct Context *C);

struct Node *op_add_mesh_raw( const char *name, int size);

// OP_ADD_BRICK

struct Set *get_current_set(struct Context *C);

struct Block *	add_default_menu(struct Context *C, const char *name);
struct Node *	add_brick_submenu( struct Context *C, struct Block *menu, struct Block *submenu, const char *name);
struct Node *	add_brick_submenu_contextual( struct Context *C, struct Block *menu, struct Block *submenu, const char *name, t_data_type context);
struct Node *	add_brick_submenu_poll( struct Context *C, struct Block *menu, struct Block *submenu, const char *name, int (* f)( struct Brick *brick));
struct Node *	add_brick_selector(struct Context *C,struct Block *block,const char *name,void *data_target,int length);
struct Node *	add_brick(struct Context *C,struct Block *block,const char *name,const char *type,const char *data_type,void *data_target);
struct Node *	add_brick_slider_int(struct Context *C,struct Block *block,const char *name,void *data_target);
struct Node *	add_brick_slider_float(struct Context *C,struct Block *block,const char *name,void *data_target);
struct Node *	add_brick_slider_float_custom(struct Context *C,struct Block *block,const char *name,void *data_target,void *(*f)(struct Brick *b));
struct Node *	add_brick_slider_int_custom(struct Context *C,struct Block *block,const char *name,void *data_target,void *(*f)(struct Brick *b));
struct Node *	add_brick_label(struct Context *C,struct Block *block,const char *name);
struct Node *	add_brick_switch(struct Context *C,struct Block *block,const char *name,void *data_target);
struct Node *	add_brick_trigger(struct Context *C,struct Block *block,const char *name,void *(*f)(struct Brick *b));
struct Node *	add_trigger(struct Context *C,const char *name,void*(* f)(struct Brick *brick));
struct Node *	add_trigger_always(struct Context *C,const char *name,void*(* f)(struct Brick *brick));
struct Node *	add_clone(struct Context *C);
struct Node *	add_maths(struct Context *C,const char *name);
struct Node *	add_brick_geometry(struct Context *C,const char *name, void *data);
struct Node *	add_brick_geo_point(struct Context *C,const char *name, void *data);
struct Node * 	add_brick_geo_edge(struct Context *C,const char *name, void *data);
struct Node * 	add_brick_geo_array(struct Context *C,const char *name, void *data);
struct Node *	add_loop(struct Context *C);
struct Node *	add_switch(struct Context *C,const char *name,void *data);
struct Node *	add_switch_custom(struct Context *C,const char *name,void *data,void *(* f)(struct Brick *brick));
struct Node *	add_label(struct Context *C,const char *name);
struct Node *	add_slider_float(struct Context *C,const char *name,void *target_data);
struct Node *	add_brick_int(struct Context *C, struct Block *block, const char *name, void *data_target);
struct Node *	add_slider_int(struct Context *C,const char *name,void *target_data);
struct Node *	add_slider_int_custom(struct Context *C,const char *name,void *target_data,void*(* f)(struct Brick *b));
struct Node *	add_slider_int_positive(struct Context *C,const char *name,void *target_data);
struct Node *	add_operator_double(struct Context *C,const char *type);
struct Node *	add_operator_single(struct Context *C,const char *type,void *(*f)(struct Brick *brick));
struct Node *	add_multiplier(struct Context *C,const char *type);
struct Node *	add_slider_float_custom(struct Context *C,const char *name,void *(*f)(struct Brick *brick));
struct Node *	add_slider_float_special(struct Context *C,const char *name,void *(*f)(struct Brick *brick));
struct Node *	add_slider_int_special(struct Context *C,const char *name,void *f(struct Brick *brick));
struct Node *	add_slider_char(struct Context *C,const char *name,void *target_data);
struct Node *	add_slider_object(struct Context *C,const char *name);
struct Node *	add_pointer_object(struct Context *C,const char *name);
struct Node *	add_slider_camera(struct Context *C,const char *name);
struct Node *	add_part_label(struct Context *C,struct Block *block,const char *name);
struct Node *	add_part_slider_float(struct Context *C,struct Block *block,const char *name,void *data_target);
struct Node *	add_part_trigger(struct Context *C,struct Block *block,const char *name,void *(*f)(struct Brick *b));
struct Node *	add_part_selector(struct Context *C,struct Block *block,const char *name,struct Node *node, t_data_type type);
struct Node *	add_part_lst(struct Context *C,struct Block *block,enum Data_Type type,const char *name,void *ptr);
struct Node *	add_part_vlst(struct Context *C,struct Block *block,enum Data_Type type,const char *name,void *ptr);
struct Node *	add_part_pointer(struct Context *C,struct Block *block,enum Data_Type type,const char *name,void *ptr);
struct Node *	add_part_vector(struct Context *C,struct Block *block,const char *name);
struct Node *	add_brick_mesh( struct Context *C,const char *name);
struct Node *	add_loop_get(struct Context *C);
struct Node *	add_get(struct Context *C);
struct Node *	add_for(struct Context *C);
struct Node *	add_vector_3d(struct Context *C);
struct Node *	add_vector_2d(struct Context *C);
struct Node *	add_stack(struct Context *C);
struct Node * 	add_if(struct Context *C);
struct Node *	add_plusplus(struct Context *C);
struct Node *	add_const(struct Context *C);
struct Node *	add_pipe(struct Context *C);
struct Node *	add_block(struct Context *C,const char *name);
void 		add_block_offset(struct Context *C, struct Block *block);
int 		op_post_quit(struct Node *node);
struct Block *	add_menu_block( struct Context *C, const char *name);

struct Node *	add_brick_bar( struct Context *C, struct Block *block, const char *name, void *f(struct Brick *b));


// OP_REGISTER


void *		find_register(const char *target,const char *name);
void 		register_set( struct Context *C);

struct Block *add_menu( struct Context *C, const char *name);
struct Block *add_submenu( struct Context *C, const char *menu_name, const char *submenu_name);


#ifdef __cplusplus
}
#endif


#endif
