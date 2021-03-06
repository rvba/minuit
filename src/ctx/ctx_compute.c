/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

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
#include "ui.h"
#include "plug.h"
#include "base.h"


void ctx_compute(t_context *C)
{
	t_process *process;
	t_set *set;
	t_node *node;
	t_link *link;

	if(C->event->debug_plug) PLUG_DEBUG = 1;
	else PLUG_DEBUG = 0;

	// For Each Process
	t_lst *lst = scene_lst_get( C->scene, dt_set);
	if( lst)
	{
	for(link = lst->first;link;link=link->next)
	{
		node = ( t_node *) link->data; 
		set = ( t_set *) node->data;

		if(set->process_delay)
		{
			if(!set->processing)
			{
				set->process = 1;
				set->process_delay = 0;
			}
		}

		// If NOT Processing
		if(set->process)
		{
			// Threading
			if(C->event->use_threading)
			{
				// Direct Exec
				if(set->frame_based)
				{
					set_exec(set);
					set->process = 1;
					set->processing = 0;
				}
				// Check Processing
				else if(set->processing)
				{
					process = engine_process_get_by_id(C->engine,set->process_id);

					if(!process->busy)
					{
						process_remove( process, C->engine);
						set->processing = 0;
						set->process = 0;
					}
				}
				// Do Processing
				else if(set->process)
				{
					process = process_add( C->engine ,"set", ctx_set_compute);
					process->data = set;
					set->process_id = process->engine_id;
					set->processing = 1;
					process_launch(process);
				}
				else
				{
				}

			}
			// Direct Exec
			else
			{
				set_exec(set);
				if(set->frame_based) set->process = 1;
				else set->process = 0;
				set->processing = 0;
			}
		}

	}
	}
}

