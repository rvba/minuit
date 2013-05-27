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

#include "context.h"
#include "block.h"

int set_draw_plug;

// OP_ACTION

void 		op_switch_color(t_context *C);
void		op_set_color(t_context *C,int color);
int 		op_exit(void);
int 		op_quit(t_node *node);
void 		op_export(t_node *node);

// OP_C

void 		op_add_global(t_context *C,t_block *block);
void 		op_init(t_context *C);
void *		find_register(const char *target,const char *name);

// OP_ADD_BRICK

t_lst *		get_target_list(t_context *C);

t_node *	add_brick_submenu(t_context *C,t_node *menu,t_node *submenu,const char *name);
t_node *	add_brick_submenu_contextual(t_context *C,t_node *menu,t_node *submenu,const char *name,t_node_type context);
t_node *	add_brick_selector(t_context *C,t_block *block,const char *name,void *data_target,int length);
t_node *	add_brick(t_context *C,t_block *block,const char *name,const char *type,const char *data_type,void *data_target);
t_node *	add_brick_slider_int(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_slider_float(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_slider_float_custom(t_context *C,t_block *block,const char *name,void *data_target,void *(*f)(t_brick *b));
t_node *	add_brick_slider_int_custom(t_context *C,t_block *block,const char *name,void *data_target,void *(*f)(t_brick *b));
t_node *	add_brick_label(t_context *C,t_block *block,const char *name);
t_node *	add_brick_switch(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_trigger(t_context *C,t_block *block,const char *name,void *(*f)(t_brick *b));
t_node *	add_trigger(t_context *C,const char *name,void*(* f)(t_brick *brick));
t_node *	add_trigger_always(t_context *C,const char *name,void*(* f)(t_brick *brick));
t_node *	add_clone(t_context *C);
t_node *	add_maths(t_context *C,const char *name);
t_node *	add_loop(t_context *C);
t_node *	add_switch(t_context *C,const char *name,void *data);
t_node *	add_switch_custom(t_context *C,const char *name,void *data,void *(* f)(t_brick *brick));
t_node *	add_label(t_context *C,const char *name);
t_node *	add_slider_float(t_context *C,const char *name,void *target_data);
t_node *	add_slider_int(t_context *C,const char *name,void *target_data);
t_node *	add_slider_int_custom(t_context *C,const char *name,void *target_data,void*(* f)(t_brick *b));
t_node *	add_slider_int_positive(t_context *C,const char *name,void *target_data);
t_node *	add_operator_double(t_context *C,const char *type);
t_node *	add_operator_single(t_context *C,const char *type,void *(*f)(t_brick *brick));
t_node *	add_multiplier(t_context *C,const char *type);
t_node *	add_slider_float_custom(t_context *C,const char *name,void *(*f)(t_brick *brick));
t_node *	add_slider_float_special(t_context *C,const char *name,void *(*f)(t_brick *brick));
t_node *	add_slider_int_special(t_context *C,const char *name,void *f(t_brick *brick));
t_node *	add_slider_char(t_context *C,const char *name,void *target_data);
t_node *	add_slider_object(t_context *C,const char *name);
t_node *	add_pointer_object(t_context *C,const char *name);
t_node *	add_slider_camera(t_context *C,const char *name);
t_node *	add_part_label(t_context *C,t_block *block,const char *name);
t_node *	add_part_slider_float(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_part_trigger(t_context *C,t_block *block,const char *name,void *(*f)(t_brick *b));
t_node *	add_part_selector(t_context *C,t_block *block,const char *name,t_node *node);
t_node *	add_part_lst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_part_vlst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_part_pointer(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_loop_get(t_context *C);
t_node *	add_get(t_context *C);
t_node *	add_for(t_context *C);
t_node *	add_vector(t_context *C);
t_node *	add_vector_3d(t_context *C);
t_node *	add_vector_2d(t_context *C);
t_node *	add_stack(t_context *C);
t_node * 	add_if(t_context *C);
t_node *	add_plusplus(t_context *C);
t_node *	add_const(t_context *C);
t_node *	add_pipe(t_context *C);


#endif
