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

#define NORTH 1
#define NORTHEAST 2
#define EAST 3
#define SOUTHEAST 4
#define SOUTH 5
#define SOUTHWEST 6
#define WEST 7
#define NORTHWEST 8
#define CENTER 9

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

typedef struct ID
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	int store;
	struct Node *node;
}t_id;

typedef int t_data_type;

#define dt_null 1
#define dt_void 2
#define dt_undefined 3
#define dt_bool 4
#define dt_int 5
#define dt_uint 6
#define dt_float 7
#define dt_char 8
#define dt_uchar 9
#define dt_dict 10
#define dt_string 11
#define dt_pointer 12
#define dt_vertex 13
#define dt_edge 14
#define dt_face 15
#define dt_mesh 16
#define dt_lst 17
#define dt_vlst 18
#define dt_camera 19
#define dt_object 20
#define dt_point 21
#define dt_selector 22
#define dt_trigger 23
#define dt_operator 24
#define dt_vector 25
#define dt_viewport 26
#define dt_binding 27
#define dt_brick 28
#define dt_block 29
#define dt_rhizome 30
#define dt_graph 31
#define dt_set 32
#define dt_geo 33
#define dt_geo_point  34
#define dt_geo_edge 35
#define dt_geo_face 36
#define dt_geo_array 37
#define dt_light 38
#define dt_screen 39
#define dt_file 40
#define dt_image 41
#define dt_material 42
#define dt_list 43
#define dt_link 44
#define dt_data 45
#define dt_texture 46
#define dt_option 47
#define dt_symbol 48
#define dt_var 49
#define dt_datum 50
#define dt_module 51

#ifdef __cplusplus
}
#endif


#endif
