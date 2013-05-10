/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "util.h"

#ifndef __MINUIT_H
#define __MINUIT_H

#define LOG_ALWAYS 1
#define LOG_MEMORY 0
#define LOG_SAVE 0
#define LOG_READ 0
#define LOG_SCENE 0
#define LOG_SCENE_NODES 0
#define LOG_CHECK 0
#define LOG_REBIND 0
#define MINUIT_VERSION 0.4

typedef struct VLst t_vlst;
typedef enum VLst_Type t_vlst_type;

typedef struct Vector t_vector;
typedef struct Vector_Class t_vector_cls;

typedef struct Node t_node;
typedef struct NodeClass t_node_class;
typedef enum  Node_Type t_node_type;

typedef struct Generic t_generic;
typedef struct Generir_Class t_generic_cls;

typedef struct Scene t_scene;

// DATA TYPE

typedef enum Data_Type t_data_type;

enum Data_Type
{
	dt_null,
	dt_int,
	dt_uint,
	dt_float,
	dt_char,
	dt_string,
	dt_pointer,
	dt_vertex,
	dt_face,
	dt_mesh,
	dt_lst,
	dt_vlst,
	dt_camera,
	dt_object,
	dt_selector,
	dt_trigger,
	dt_operator,
	dt_vector,
	dt_viewport
};

enum Node_Type
{
	nt_generic,
	nt_null,
	nt_mesh,
	nt_block,
	nt_brick,
	nt_light,
	nt_object,
	nt_screen,
	nt_file,
	nt_image,
	nt_material,
	nt_list,
	nt_link,
	nt_data,
	nt_texture,
	nt_var,
	nt_option,
	nt_vlst,
	nt_camera,
	nt_dict,
	nt_symbol,
	nt_vector,
	nt_viewport,
	nt_set,
};
	



// GENERIC

/*
block plug camera data engine light vlst texture
object screen txt image link lst
*/

struct Generic_Class
{
	char cls_type[_NAME_];
};

struct Generic
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_generic_cls *cls;
};

// NODE CLASS

struct NodeClass
{
	t_node_type type;
	size_t size;
	t_lst *lst;

	int  (* make)(t_node *node);
	void (* build)(t_node *node,const char *name);
	void (* link)(t_node *node);
	void (* del)(t_node *node);
	void (* init)(t_node *node);
	void (* free)(t_scene *sc,t_node *node);

	void (* set_state_selected)(t_node *node,int state);
	int (* is_mouse_over)(t_node *node);
	void *(* get_ref)(t_node *node, const char *ref);
};

// NODE

struct Node
{
	int id;
	int id_old;
	int id_chunk;
	int id_chunk_self;
	void *id_ptr;
	void *id_ptr_old;
	int users;

	t_node_type type;
	t_node_class *cls;

	void *data;  
};

// SCENE

struct Scene 
{
	// state
	int is_ready;
	int store;

	// id
	int id; 

	// color
	int step;
	int color[3];	
	int use_tmp_colors;
	
	t_lst *nodes;

	t_lst *files;
	t_lst *textures;
	t_lst *materials;
	t_lst *blocks;
	t_lst *bricks;
	t_lst *meshes;
	t_lst *lights;
	t_lst *objects;
	t_lst *cameras;
	t_lst *screens; 
	t_lst *images;
	t_lst *datas;
	t_lst *vlst;
	t_lst *lists;
	t_lst *links;
	t_lst *dicts;
	t_lst *symbols;
	t_lst *vectors;
	t_lst *viewports;
	t_lst *sets;

	t_lst *vars;

	// selections
	t_node *selected;
	t_lst *selections;
	t_lst *stack; 			 

	// tmp
	t_lst *tmp_colors;
	t_lst *tmp_node;
	t_lst *tmp_data;
};

char *node_name_get(t_node_type type);



#endif




