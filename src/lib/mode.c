/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "mode.h"

t_module *module_new(char *name,void *data)
{
	t_module *module=(t_module *)malloc(sizeof(t_module));
	strncpy(module->name,name,10);
	module->data=data;
	module->update=NULL;
	return module;
}

t_module *mode_module_add(t_mode *mode,char *name,void *data)
{
	int is_free=1;
	t_link *link;
		
	if(mode->modules->first)
	{
		for(link=mode->modules->first;link;link=link->next)
		{
			t_module *module=link->data;
			if(is(module->name,"name"))
			{
				printf("module %s exists\n",name);
				is_free=0;
			}
		}
	}

	if(is_free)
	{
		t_module *module=module_new(name,data);
		lst_add(mode->modules,module,module->name);
		return module;
	}
	else
	{
		return NULL;
	}
}

t_module *mode_module_get(t_mode *mode,char *name)
{
	t_link *link;
	for(link=mode->modules->first;link;link=link->next)
	{
		t_module *module=link->data;
		if(is(module->name,name))
			return module;
	}
	return NULL;
}

// update all modules
void mode_update(t_mode *mode)
{
	t_link *l=mode->modules->first;
	if(l)
	{
		for(;l;l=l->next)
		{
			t_module *m=l->data;
			if(m->update)m->update(m);
		}
	}
}

void mode_init(t_mode *mode)
{
	mode->modules=lst_new("lst");
}

t_mode *mode_new(void)
{
	t_mode *mode=(t_mode *)malloc(sizeof(t_mode));
	mode->modules=NULL;
	mode->init=mode_init;
	mode->update=mode_update;
	return mode;
}
	
