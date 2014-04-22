/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "util.h"
#include "material.h"
#include "scene.h"
#include "node.h"
#include "memory.h"

// CLONE

t_material *material_clone(t_material *material)
{
	if(material)
	{
		t_material *clone = material_new(material->id.name);
		vcp3i(clone->idcol,material->idcol);
		clone->id_node = material->id_node;

		vcp3f(clone->color,material->color);
		vcp3f(clone->specular,material->specular);
		vcp3f(clone->shininess,material->shininess);
		vcp3f(clone->s_color,material->s_color);

		return clone;
	}
	else
	{
		return NULL;
	}
}

// REF

void *material_get_ref(t_material *material, const char *ref)
{
	void *p;

	if(is(ref,"red"))  			p=&material->color[0]; 
	else if(is(ref,"green"))  		p=&material->color[1]; 
	else if(is(ref,"blue"))  		p=&material->color[2]; 
	else if(is(ref,"alpha"))  		p=&material->color[3]; 
	else
	{
		printf("[ERROR material_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}


// REBIND

void material_rebind(t_scene *sc,void *ptr)
{
	//t_material *material=(t_material *)ptr;
}

// FREE

void _material_free(t_material *material)
{
	mem_free( material, sizeof( t_material));
}

void material_free(t_material *material)
{
	mem_free( material, sizeof( t_material));
}

// NEW

t_material *material_new(const char *name)
{
	t_material *material = (t_material *)mem_malloc(sizeof(t_material));

	id_init(&material->id, name);

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
