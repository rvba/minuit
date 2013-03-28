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

char *node_name_null;
char *node_name_generic;
char *node_name_brick;
char *node_name_mesh;
char *node_name_block;
char *node_name_light;
char *node_name_object;
char *node_name_screen;
char *node_name_file;
char *node_name_image;
char *node_name_material;
char *node_name_list;
char *node_name_link;
char *node_name_data;
char *node_name_texture;
char *node_name_var;
char *node_name_option;
char *node_name_vlst;
char *node_name_camera;
char *node_name_dict;
char *node_name_symbol;
char *node_name_vector;

t_node*		node_load(void *ptr,const char *type);
void		node_init(t_node *node,t_node_type type);
t_node *	node_new(t_node_type type);
t_node *	lst_find_node(t_lst *lst,const char *name);
void 		lst_node_delete_all(t_lst *lst);
t_node *	node_clone(t_node *src);

int 		dlink(const char *type,void *ptr);

#endif

