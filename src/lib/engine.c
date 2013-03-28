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

t_process *engine_process_get(t_engine *engine,char *name)
{
	void *ptr;
	ptr=lst_get(engine->processes,name);
	if(ptr)
	{
		return (t_process *)ptr;
	}
	else
	{
		return NULL;
	}
}

t_engine *engine_new(const char *name)
{
	t_engine *engine = (t_engine *)malloc(sizeof(t_engine));

	engine->id=0;
	engine->id_chunk=0;
	set_name(engine->name,name);
	engine->processes=lst_new("lst");
	engine->with_global_limit=ENGINE_WITH_GLOBAL_LIMIT;
	engine->global_freq=ENGINE_GLOBAL_FREQ;
	engine->tot_processes=0;

	return engine;
}
