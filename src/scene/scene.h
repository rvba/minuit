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

#include "base.h"

struct Node;
struct NodeClass;
struct Lst;

typedef struct Scene 
{
	// state
	int is_ready;
	int store;
	int has_generic_viewport;
	int store_stack;

	// id
	int id; 

	// color
	int step;
	int color[3];	
	int use_tmp_colors;
	int edit_mode;
	int debug_all;
	
	struct Lst *nodes;
	struct Lst *classes;

	// selections
	struct Node *selected;
	struct Node *hover;
	t_data_type hover_type;
	struct Lst *selections;
	struct Lst *stack; 			 

	struct Node *selection;
	struct Node *selection_swap;

	// tmp
	struct Lst *tmp_colors;
	struct Lst *tmp_node;
	struct Lst *tmp_data;

	// Data
	const char *(* data_name)(int type);

}t_scene;


struct Node *	scene_get_data(t_scene *sc,void *ptr);
struct Node *	scene_get_var(t_scene *sc,void *ptr);
struct Node *	scene_get_node_by_id( t_scene *scene, int id);
struct Node*	scene_get_node_by_type_name(t_scene *sc, t_data_type type,const char *name);
t_scene*	scene_get(void);

void 		scene_color_tmp_reset(t_scene *scene);
void 		scene_color_get(t_scene *scene,int *color);
void		scene_color_set(t_scene *sc,struct Node *node);
int 		scene_id_get(t_scene *sc);
void		scene_color_switch_mode(t_scene *sc);
void 		scene_switch_edit_mode(t_scene *scene, int state);

void 		scene_node_load(t_scene *sc,struct Node *node);
void 		scene_data_node_load(t_scene *sc,struct Node *node);
void 		scene_node_free(t_scene *sc,struct Node *node);
int		scene_node_delete(t_scene *sc,struct Node *node);
struct Node*	scene_node_exists(t_scene *sc, t_data_type type,const char *name);
void		scene_cleanup(t_scene *sc);

struct Node *	scene_add(t_scene *sc, t_data_type type,const char *name);
struct Node *	scene_add_data(t_scene *sc,const char *type,const char *target,const char *name,void *ptr);
struct Node *	scene_add_data_node(t_scene *sc,const char *type,const char *target,const char *name,void *ptr);
struct Node *	scene_add_ref(t_scene *sc,const char *type,const char *target,const char *name,void *ptr,void *ref);
void 		scene_add_data_var(t_scene *sc,const char *name,const char *name_var,int size,void *ptr);

void 		scene_free(t_scene *sc);
void 		scene_init(t_scene *scene);
int 		scene_delete_by_id(t_scene *sc,const char *type,int id);
void 		scene_data_load(t_scene *sc);
//void 		scene_struct_delete(t_scene *sc,void *ptr);


void scene_remove_data_node(t_scene *sc,void *ptr);
void scene_remove_ref(t_scene *sc,void *ptr);

void scene_log(t_scene *sc);


void scene_vector_free(t_scene *sc,struct Node *node);
void scene_light_free(t_scene *sc,struct Node *node); 
void scene_texture_free(t_scene *sc,struct Node *node);
void scene_list_free(t_scene *sc,struct Node *node);
void scene_camera_free(t_scene *sc,struct Node *node);
void scene_mesh_free(t_scene *sc,struct Node *node);
void scene_brick_free(t_scene *sc,struct Node *node);
void scene_block_free(t_scene *sc,struct Node *node);
void scene_object_free(t_scene *sc,struct Node *node);
void scene_data_free(t_scene *sc,struct Node *node);
void scene_vlst_free(t_scene *sc,struct Node *node);
void scene_link_free(t_scene *sc,struct Node *node);
void scene_dict_free(t_scene *sc,struct Node *node);
void scene_symbol_free(t_scene *sc,struct Node *node);
void scene_var_free(t_scene *sc,struct Node *node);
void scene_generic_free(t_scene *sc,struct Node *node);
void scene_viewport_free(t_scene *sc,struct Node *node);
void scene_set_free(t_scene *sc,struct Node *node);

void scene_screen_free(t_scene *sc,struct Node *node);
void scene_file_free(t_scene *sc,struct Node *node) ;
void scene_image_free(t_scene *sc,struct Node *node);
void scene_material_free(t_scene *sc,struct Node *node); 
void scene_option_free(t_scene *sc,struct Node *node) ;

t_scene *scene_new(void);
void scene_store(t_scene *scene, int val);

void scene_delete( t_scene *sc, void *data);

struct NodeClass *scene_class_get( t_scene *scene, t_data_type type);

void scene_class_init( t_scene *scene, t_data_type type, struct NodeClass *cls);

struct Lst *scene_lst_get( t_scene *sc, t_data_type type);


#endif
