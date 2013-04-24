/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

t_engine_cls cls_engine=
{
	.cls_type="engine",
};

void engine_quit(t_engine *engine)
{
	if(engine->tot_processes>0)
	{
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
		if( is(process->name, name))
		{
			return process;
		}
	}
	 
	return NULL;
}

void engine_cleanup(t_engine *engine)
{
	t_link *link;
	t_process *process;

	if(engine->garbage->first)
	{
		lst_show(engine->garbage);
		for(link = engine->garbage->first; link; link = link->next)
		{
			process = link->data;

			if(process->done)
			{
				lst_link_delete(engine->garbage,link);
				list_remove_by_name(engine->processes,process->name);
				process_free(process);
				break;
			}
		}

		engine_cleanup(engine);
	}
}

void engine_process_remove(t_engine *engine, const char *name)
{
	t_process *process = engine_process_get(engine, name);
	process->exit = 1;
	lst_add(engine->garbage,process,name);
}

t_engine *engine_new(const char *name)
{
	t_engine *engine = (t_engine *)malloc(sizeof(t_engine));

	engine->id=0;
	engine->id_chunk=0;
	set_name(engine->name,name);
	engine->processes=lst_new("lst");
	engine->garbage = lst_new("lst");
	engine->with_global_limit=ENGINE_WITH_GLOBAL_LIMIT;
	engine->global_freq=ENGINE_GLOBAL_FREQ;
	engine->tot_processes=0;

	return engine;
}
