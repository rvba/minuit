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

#include "util.h"
#include "minuit.h"

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
t_node *	lst_find_node(t_lst *lst,const char *name);
void 		lst_node_delete_all(t_lst *lst);
t_node *	node_clone(t_node *src);

int 		dlink(const char *type,void *ptr);

#endif

