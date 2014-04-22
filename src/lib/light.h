/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Block;
struct Scene;
struct Node;

typedef struct Light t_light;

// LIGHT

struct Light
{
	t_id id;

	int idcol[3];
	int id_gl;
	int is_selected;
	float pos[4];
	float diffuse[4];
	float specular[4];

	struct Block *menu;
};

void 		light_show(t_light *light);
void 		light_free(t_light *light);
t_light*	light_new(const char *name);
void		light_rebind(struct Scene *sc,void *ptr);
struct Node*		light_make(const char *name);

#ifdef __cplusplus
}
#endif

#endif
