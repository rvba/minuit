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
}t_id;

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

}t_node_type;



#endif
