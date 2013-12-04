/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "node.h"
#include "op.h"
#include "engine.h"
#include "process.h"
#include "list.h"
#include "context.h"
#include "app.h"
#include "memory.h"

t_engine_cls cls_engine=
{
	.cls_type="engine",
};

void engine_quit(t_engine *engine)
{
	if(engine->process_count>0)
	{
		printf("Engine Quit\n");

		engine_show(engine);
		t_link *link;
		t_process *process;

		for(link=engine->processes->first;link;link=link->next)
		{
			process=link->data;
			process->exit=1;
		}
	}
}

t_process *engine_process_get(t_engine *engine,const char *name)
{
	t_link *link;
	t_process *process;

	for(link = engine->processes->first; link; link = link->next)
	{
		process = link->data;
		if( is(process->id.name, name))
		{
			return process;
		}
	}
	 
	return NULL;
}

t_process *engine_process_get_by_id(t_engine *engine, int id)
{
	t_link *link;
	t_process *process;

	for(link = engine->processes->first; link; link = link->next)
	{
		process = link->data;
		if(process->engine_id == id)
		{
			return process;
		}
	}
	 
	return NULL;
}

void engine_show(t_engine *engine)
{
	printf("[engine] show\n");
	printf("[engine] processes\n");
	t_link *link;
	for(link=engine->processes->first;link;link=link->next)
	{
		t_process *process = link->data;
		printf("%d %s\n",process->engine_id, process->id.name);
	}

	printf("[engine] garbage\n");
	for(link=engine->garbage->first;link;link=link->next)
	{
		t_process *process = link->data;
		printf("%d %s\n",process->engine_id, process->id.name);
	}
}


void engine_cleanup(t_engine *engine)
{
	t_link *link;
	t_process *process;

	if(engine->garbage->first)
	{
		int do_loop = 0;
		for(link = engine->garbage->first; link; link = link->next)
		{
			process = link->data;

			if(process->done)
			{
				list_remove_by_name(engine->processes, process->id.name);
				list_remove_by_name(engine->garbage, process->id.name);
				process_free(process);

				do_loop = 1;
				break;
			}
		}

		if(do_loop) engine_cleanup(engine);
	}
}

// REMOVE

void engine_process_remove(t_engine *engine, t_process *process)
{
	process->exit = 1;
	engine->process_count--;
	lst_add(engine->garbage, process, process->id.name);
}

// ADD

void engine_process_add(t_engine *engine, t_process *process)
{
	lst_add(engine->processes, process, process->id.name);
	engine->process_count++;
	engine->process_id++;
	process->engine_id = engine->process_id;
}

// NEW

t_engine *engine_new(const char *name)
{
	t_engine *engine = (t_engine *)mem_malloc(sizeof(t_engine));

	id_init(&engine->id, name);

	engine->processes=lst_new("lst");
	engine->garbage = lst_new("lst");
	engine->with_global_limit=ENGINE_WITH_GLOBAL_LIMIT;
	engine->global_freq=ENGINE_GLOBAL_FREQ;
	engine->process_count=0;
	engine->process_id = 0;

	return engine;
}
