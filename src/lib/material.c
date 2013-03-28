/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"


// REBIND

t_material *material_rebind(t_scene *sc,void *ptr)
{
	t_material *material=(t_material *)ptr;
	check_init("MATERIAL",material->name);
	check_check("MATERIAL",material->name);
	return material;
}

// NEW

t_material *material_new(const char *name)
{
	t_material *material = (t_material *)malloc(sizeof(t_material));

	set_name(material->name,name);
	material->users=0;

	material->specular[0]=1;
	material->specular[1]=1;
	material->specular[2]=1;
	material->specular[3]=1;

	material->shininess[0]=50;
	material->shininess[1]=50;
	material->shininess[2]=50;
	material->shininess[3]=50;

	material->color[0]=.5;
	material->color[1]=.5;
	material->color[2]=.5;
	material->color[3]=1;

	material->s_color[0]=0;
	material->s_color[1]=0;
	material->s_color[2]=0;
	material->s_color[3]=0;

	return material;
}
