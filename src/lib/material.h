/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;

typedef struct Material t_material;

struct Material
{
	t_id id;

	int idcol[3];
	int id_node;

	float color[4];
	float specular[4];
	float shininess[4];
	float s_color[4];
};

t_material *	material_clone(t_material *material);
void *		material_get_ref(t_material *material, const char *ref);
void 		material_free(t_material *material);
void		 _material_free(t_material *material);
void *		material_new(const char *name);
void		material_rebind(struct Scene *sc,void *ptr);

#ifdef __cplusplus
}
#endif

#endif
