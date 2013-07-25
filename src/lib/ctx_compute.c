/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "list.h"
#include "ctx.h"
#include "app.h"
#include "process.h"
#include "engine.h"
#include "term.h"
#include "object.h"
#include "clock.h"
#include "system.h"
#include "event.h"
#include "set.h"
#include "ui.h"


void ctx_compute(t_context *C)
{
	t_process *process;
	t_set *set;
	t_node *node;
	t_link *link;

	// For Each Process
	for(link = C->scene->sets->first;link;link=link->next)
	{
		node = link->data; 
		set = node->data;

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
					set->processing = 0;
					set->process = 0;
				}
				// Check Processing
				else if(set->processing)
				{
					process = engine_process_get_by_id(C->engine,set->process_id);

					if(!process->busy)
					{
						process_remove(process);
						set->processing = 0;
						set->process = 0;
					}
				}
				// Do Processing
				else if(set->process)
				{
					process = process_add(C,"set",ctx_set_compute);
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
				set->process = 0;
				set->processing = 0;
			}
		}

	}
}

