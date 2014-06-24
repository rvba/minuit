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


#ifdef __cplusplus
extern "C" {
#endif

#define MILLION 1000000

#define APP_VERSION "0.6.4"

#define _NAME_ 64
#define _NAME_LONG_ 128
#define _PATH_ 150
#define _EXT_ 10
#define _STATE_ 20
#define _LIST_ 10

#define MINUIT_VERSION 0.6

#define DEBUG_PROCESS 0
#define DEBUG_CONSOLE 0
#define DEBUG_TERMINAL 0
#define DEBUG_LOOP 0
#define DEBUG_KEY 0
#define DEBUG_PLUG 1 
#define DEBUG_ALL 0
#define DEBUG_TO_FILE 0

#define PI 3.14159655f

#define BIN_MAX 10000

#define PROCESS_LIMIT 0.01

#define N_w 99

#define NORTH 1
#define NORTHEAST 2
#define EAST 3
#define SOUTHEAST 4
#define SOUTH 5
#define SOUTHWEST 6
#define WEST 7

#define UP 1
#define DOWN 2

typedef struct ID
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	int store;
	struct Node *node;
}t_id;

typedef enum Data_Type
{
	dt_null,
	dt_undefined,
	dt_bool,
	dt_int,
	dt_uint,
	dt_float,
	dt_char,
	dt_uchar,
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
	dt_point,
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
	dt_datum,
	dt_module,

}t_data_type;

#ifdef __cplusplus
}
#endif


#endif
