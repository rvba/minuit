/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "op.h"
#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "list.h"
#include "app.h"
#include "term.h"
#include "object.h"
#include "clock.h"
#include "event.h"
#include "set.h"
#include "memory.h"

void *ctx_compute_rhizome(void *data)
{
	t_context *C=ctx_get();
	t_process *process=(t_process *)data;

	process->busy=1;
	ctx_set_exec(C);
	process->exit = 1;
	process->busy=0;

	return NULL;
}

void *ctx_set_compute(void *data)
{
	t_process *process=(t_process *)data;
	t_set *set = ( t_set *) process->data;

	process->busy=1;
	set_exec(set);
	process->busy=0;

	return NULL;
}

void *ctx_compute_objects(void *data)
{
	t_context *C=ctx_get();
	t_process *process=(t_process *)data;
	if(APP_DEBUG)
	{
		char msg[20];
		sprintf(msg,"%d compute",C->app->frame);
		term_log( msg);
	}

	t_node *node_object;
	t_link *link_object;

	if(C->scene->is_ready)
	{
		t_lst *lst = scene_lst_get( C->scene, dt_object);
		for(link_object=lst->first;link_object;link_object=link_object->next)
		{
			node_object = ( t_node *) link_object->data;
			t_object *object = ( t_object *) node_object->data;
			if(!object)
			{
				printf("[ERROR ctx_compute] No data\n");
				break;
			}
			else
			{
				if(object->update)
				{
					object->update(node_object);
				}
			}
		}
	}

	process->cycle++;

	return NULL;
}



void ctx_thread_init(t_context *C)
{
}




