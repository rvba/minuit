/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define _NAME_ 64
#define _NAME_LONG_ 128
#define _PATH_ 150
#define _EXT_ 10
#define _STATE_ 20
#define _LIST_ 10

#define MINUIT_VERSION 0.4

#define DEBUG_PROCESS 0
#define DEBUG_CONSOLE 1
#define DEBUG_TERMINAL 1
#define DEBUG_LOOP 0
#define DEBUG_KEY 0
#define DEBUG_PLUG 1 
#define DEBUG_ALL 0
#define DEBUG_TO_FILE 0

#define PI 3.14159655f

#define BIN_MAX 10000

#define PROCESS_LIMIT 0.01

#define N_w 99

typedef struct ID
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	int store;
}t_id;

/*
typedef enum Node_Type
{
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
	nt_binding,
	nt_rhizome,
	nt_graph,
	nt_geo,
	nt_geo_point,
	nt_geo_edge,
	nt_geo_array,

}t_node_type;
*/

typedef enum Data_Type
{
	dt_null,
	dt_bool,
	dt_int,
	dt_uint,
	dt_float,
	dt_char,
	dt_dict,
	dt_string,
	dt_pointer,
	dt_vertex,
	dt_edge,
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
	dt_viewport,
	dt_binding,
	dt_brick,
	dt_block,
	dt_rhizome,
	dt_graph,
	dt_set,
	dt_geo,
	dt_geo_point,
	dt_geo_edge,
	dt_geo_face,
	dt_geo_array,
	dt_light,
	dt_screen,
	dt_file,
	dt_image,
	dt_material,
	dt_list,
	dt_link,
	dt_data,
	dt_texture,
	dt_option,
	dt_symbol,
	dt_var,

}t_data_type;



#endif
