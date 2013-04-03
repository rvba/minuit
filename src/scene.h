/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SCENE_H
#define __SCENE_H

#include "minuit.h"
#include "light.h"
#include "object.h"
#include "screen.h"
#include "list.h"
#include "data.h"
#include "memory.h"
#include "obj.h"
#include "camera.h"
#include "dict.h"
#include "vector.h"

t_node *	scene_get_data(t_scene *sc,void *ptr);
t_node *	scene_get_var(t_scene *sc,void *ptr);
t_scene*	scene_get(void);
void 		scene_color_get(t_scene *scene,int *color);
void		scene_color_set(t_scene *sc,t_node *node);
int 		scene_id_get(t_scene *sc);
void		scene_color_switch_mode(t_scene *sc);
void 		scene_node_load(t_scene *sc,t_node *node);
void 		scene_data_node_load(t_scene *sc,t_node *node);
void 		scene_node_free(t_scene *sc,t_node *node);
int		scene_node_delete(t_scene *sc,t_node *node);
t_node *	scene_node_get_by_id_global(t_scene *sc,int id);
t_node*		scene_node_get(t_scene *sc,const char *type,const char *name);
void		scene_cleanup(t_scene *sc);

t_node*		scene_add(t_scene *sc,t_node_type type,const char *name);
t_node *	scene_add_data(t_scene *sc,const char *type,const char *target,const char *name,void *ptr);
t_node *	scene_add_data_node(t_scene *sc,const char *type,const char *target,const char *name,void *ptr);
t_node *	scene_add_ref(t_scene *sc,const char *type,const char *target,const char *name,void *ptr,void *ref);
void 		scene_add_data_var(t_scene *sc,const char *name,const char *name_var,int size,void *ptr);

void 		scene_free(t_scene *sc);
t_scene*	scene_init(void);
int 		scene_delete_by_id(t_scene *sc,const char *type,int id);
void 		scene_data_load(t_scene *sc);
void 		scene_struct_delete(t_scene *sc,void *ptr);


void scene_remove_data_node(t_scene *sc,void *ptr);
void scene_remove_ref(t_scene *sc,void *ptr);

void scene_log(t_scene *sc);


void scene_vector_free(t_scene *sc,t_node *node);
void scene_light_free(t_scene *sc,t_node *node); 
void scene_texture_free(t_scene *sc,t_node *node);
void scene_list_free(t_scene *sc,t_node *node);
void scene_camera_free(t_scene *sc,t_node *node);
void scene_mesh_free(t_scene *sc,t_node *node);
void scene_brick_free(t_scene *sc,t_node *node);
void scene_block_free(t_scene *sc,t_node *node);
void scene_object_free(t_scene *sc,t_node *node);
void scene_data_free(t_scene *sc,t_node *node);
void scene_vlst_free(t_scene *sc,t_node *node);
void scene_link_free(t_scene *sc,t_node *node);
void scene_dict_free(t_scene *sc,t_node *node);
void scene_symbol_free(t_scene *sc,t_node *node);
void scene_var_free(t_scene *sc,t_node *node);
void scene_generic_free(t_scene *sc,t_node *node);

void scene_screen_free(t_scene *sc,t_node *node);
void scene_file_free(t_scene *sc,t_node *node) ;
void scene_image_free(t_scene *sc,t_node *node);
void scene_material_free(t_scene *sc,t_node *node); 
void scene_option_free(t_scene *sc,t_node *node) ;

t_node *scene_struct_get(t_scene *sc,void *ptr);

#endif
