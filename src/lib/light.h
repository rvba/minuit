/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "minuit.h"
#include "block.h"

#ifndef __LIGHT_H_
#define __LIGHT_H_

typedef struct Light t_light;

// LIGHT

struct Light
{
	int id; //prev id_gl
	int id_chunk;
	short users;
	char name[_NAME_];

	int idcol[3];
	int id_gl;
	int is_selected;
	float pos[4];
	float diffuse[4];
	float specular[4];

	t_block *menu;
};

void 		light_show(t_light *light);
void 		light_free(t_light *light);
t_light*	light_new(const char *name);
t_light*	light_rebind(t_scene *sc,void *ptr);
t_node*		light_make(const char *name);


#endif
