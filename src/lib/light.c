/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "util.h"
#include "light.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "list.h"
#include "memory.h"

t_light *light_rebind(t_scene *sc,void *ptr)
{
	t_light *light=(t_light *)ptr;
	return light;
}

t_node *light_make(const char *name)
{
	t_context *C=ctx_get();
	t_node *node=scene_add(C->scene,dt_light,name);
	t_light *light=node->data;

	// set pos
	vset3f(light->pos,3,3,3);

	// set id 
	light->id_gl=C->scene->lights->count;

	return node;
}

void light_free(t_light *light)
{
	mem_free( light, sizeof( t_light));
}

t_light *light_new(const char *name)
{
	t_light *light = (t_light *)mem_malloc(sizeof(t_light));

	id_init(&light->id, name);
	
	light->diffuse[0]=1.1;
	light->diffuse[1]=1;
	light->diffuse[2]=1;
	light->diffuse[3]=1;

	light->specular[0]=1;
	light->specular[1]=1;
	light->specular[2]=1;
	light->specular[3]=1;

	light->is_selected=0;

	light->menu=NULL;
	return light;
}
