/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "action.h"
#include "context.h"
#include "scene.h"
#include "memory.h"

void action_free(t_action *action)
{
	t_context *C = ctx_get();
	t_scene *sc = C->scene;
	if(action->args) scene_struct_delete(sc,action->args);
	mem_free( action, sizeof( t_action));
}

t_action *action_new(const char *name)
{
	t_action *action = (t_action *)mem_malloc(sizeof(t_action));
	
	action->act = NULL;
	action->args = NULL;

	return action;
}
