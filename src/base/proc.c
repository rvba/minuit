/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

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
			if(process->limit && process->clock->limit)
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

void process_launch( t_process *process)
{
	pthread_create( &process->thread, NULL, process->loop, process);
}

void process_remove( t_process *process, t_engine *engine)
{
	engine_process_remove( engine, process);
}

t_process *process_add( t_engine *engine, const char *name, void *(* f)(void *d))
{
	t_process *process = process_new( name, f);
	process->clock->limit=.1;
	engine_process_add( engine, process);

	return process;
}

t_process *process_start( t_engine *engine, const char *name, void *(* f)(void *d), void *data, float clock)
{
	t_process *process = process_add( engine, name, f);
	process->data = data;
	process->clock->limit = clock;
	process_launch( process);
	return process;
}

void process_free(t_process *process)
{
	clock_free(process->clock);
	mem_free( process, sizeof( t_process));
}

t_process *process_new(const char *name,void*(* func)(void *data))
{
	t_process *process = (t_process *)mem_malloc(sizeof(t_process));

	id_init(&process->id, name);

	process->clock = clock_new( "clock");
	process->limit=1;
	process->play=1;
	process->exit=0;
	process->cycle=0;
	process->done = 0;
	process->busy = 0;

	process->func=func;
	process->loop=process_loop;
	process->exec=func;

	process->data = NULL;
	process->engine_id = 0;

	return process;
}
