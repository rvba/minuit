/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

void *ctx_compute(void *data)
{
	t_context *C=ctx_get();
	t_process *process=(t_process *)data;
	if(APP_DEBUG)
	{
		char msg[20];
		sprintf(msg,"%d compute",C->app->frame);
		term_print(C->term,msg);
	}

	t_node *node_object;
	t_link *link_object;

	if(C->scene->is_ready)
	{
		for(link_object=C->scene->objects->first;link_object;link_object=link_object->next)
		{
			node_object=link_object->data;
			t_object *object = node_object->data;
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

void *process_loop(void *data)
{
	t_process *process = (t_process *)data;
	
	while(1)
	{
		// EXIT process
		if(process->exit)
		{
			process->done = 1;
			pthread_exit(NULL);
		}
		// or EXEC
		else
		{
			// WITH TIME LIMIT
			if(process->limit)
			{
				// get time
				gettimeofday(&process->clock->now,NULL);
				// get delta
				process->clock->delta = 
					(process->clock->now.tv_sec -process->clock->before.tv_sec)
					+
					(process->clock->now.tv_usec - process->clock->before.tv_usec)*0.000001f;

				// test delta < limit
				if (process->clock->delta < process->clock->limit) 
				{
					// check frequence
					if(process->clock->limit - process->clock->delta < process->clock->freq)  
					{
						// sleep
						usleep((int)((process->clock->limit - process->clock->delta)*1000000.0f));
					}
				}
				else 
				{
					if(process->play)
						process->exec(process);

					process->clock->before = process->clock->now;
				}
			}
			// WITHOUT TIME LIMIT
			else
			{
				if(process->play)
				{
					process->exec(process);
				}
			}
		}
	}

	return NULL;
}


// PROCESS


void process_launch(t_process *process)
{
	pthread_create(&process->thread,NULL,process->loop,process);
	term_log("p %s", process->name);
}

t_process *process_add(t_context *C,char *name,void*(* func)(void *ptr))
{
	t_process *process = process_new(name,func);
	lst_add(C->engine->processes,process,name);

	return process;
}

void ctx_thread_init(t_context *C)
{
	t_process *process = process_new("main",ctx_compute);
	
	process->clock->limit=.1;
	process_launch(process);
	lst_add(C->engine->processes,process,"process_main");
}

void process_free(t_process *process)
{
	clock_free(process->clock);
	free(process);
}

t_process *process_new(char *name,void*(* func)(void *data))
{
	t_process *process = (t_process *)malloc(sizeof(t_process));

	process->id = 0;
	process->id_chunk = 0;
	process->users = 0;
	
	set_name(process->name,name);

	process->clock=clock_new();
	process->limit=1;
	process->play=1;
	process->exit=0;
	process->cycle=0;
	process->done = 0;
	process->busy = 0;

	process->func=func;
	process->loop=process_loop;
	process->exec=func;

	return process;
}



