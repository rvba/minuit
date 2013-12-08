/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "op.h"
#include "context.h"
#include "node.h"
#include "scene.h"
#include "ctx.h"
#include "dict.h"
#include "list.h"
#include "memory.h"

void symbol_show(t_symbol *symbol)
{
	char *name=symbol->name;

	switch(symbol->data_type)
	{
		case dt_int:printf("(int\t)%s:(%d)\n",name,drf_int(symbol->data));break;
		case dt_float:printf("(float\t)%s:(%f)\n",name,drf_float(symbol->data));break;
		case dt_string:printf("(string\t)%s:(%s)\n",name,drf_string(symbol->data));break;
		case dt_pointer:printf("(pointer\t)%s:(%p)\n",name,symbol->data);break;
		case dt_lst:printf("(lst\t)%s\n",name);lst_show(symbol->data);break;
		default: printf("[WARNING symbol_show] Unknown type\n");
	}
}
			

void dict_show(t_dict *dict)
{
	t_lst *lst=dict->symbols;
	t_link *l;
	t_symbol *symbol;

	printf("DICT\t(%s)\n", dict->id.name);
	printf("count:%d\n", dict->count);
	if(lst)
	{
		for(l=lst->first;l;l=l->next)
		{
			symbol=l->data;
			symbol_show(symbol);
		}
	}
}

// ADD

t_symbol *symbol_make( const char *name)
{
	t_scene *sc = ctx_scene_get();
	t_symbol *symbol;
	t_node *node= scene_add( sc, dt_symbol, name);
	symbol = node->data;

	return symbol;
}

// DICT SYMBOL ADD

t_symbol *dict_symbol_add(t_dict *dict,const char *name,t_data_type type,void *data)
{
	t_symbol *symbol = symbol_make( name);

	symbol->data_type=type;
	symbol->data=data;

	list_add_data(dict->symbols,symbol);

	dict->count++;

	return symbol;
}

t_symbol *dict_pop(t_dict *dict,const char *name)
{
	t_lst *lst=dict->symbols;
	t_link *l;
	t_symbol *symbol;

	for(l=lst->first;l;l=l->next)
	{
		symbol=l->data;
		if(is(symbol->name,name))
		{
			return symbol;
		}
	}
	
	return NULL;
}

void *dict_pop_data(t_dict *dict,const char *name)
{
	t_symbol *symbol = dict_pop(dict,name);
	return symbol->data;
}

t_dict *dict_make( const char *name)
{
	t_scene *sc = ctx_scene_get();
	t_dict *dict;
	t_node *node_dict = scene_add( sc, dt_dict, name);
	dict=node_dict->data;

	// new list
	dict->symbols = list_make( dt_dict, "dict");

	return dict;
}

// REBIND

t_symbol *symbol_rebind(t_scene *sc,void *ptr)
{
	t_symbol *symbol=(t_symbol *)ptr;

	rebind(sc,"symbol","data",(void **)&symbol->data);

	return symbol;
}


t_dict *dict_rebind(t_scene *sc,void *ptr)
{
	t_dict *dict=(t_dict *)ptr;

	rebind(sc,"dict","symbols",(void **)&dict->symbols);

	return dict;
}

// NEW

t_symbol *symbol_new(const char *name)
{
	t_symbol *symbol = (t_symbol *)mem_malloc(sizeof(t_symbol));

	id_init( &symbol->id, name);
	symbol->id_chunk=0;
	symbol->users=0;
	set_name(symbol->name,name);

	symbol->data_type=dt_null;
	symbol->data=NULL;

	return symbol;
}
	

t_dict *dict_new(const char *name)
{
	t_dict *dict = (t_dict *)mem_malloc(sizeof(t_dict));

	id_init(&dict->id, name);

	dict->symbols=NULL;
	dict->count = 0;

	return dict;
}

// FREE

void symbol_free(t_symbol *symbol)
{
	mem_free( symbol, sizeof( t_symbol));
}

void dict_free(t_dict *dict)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	// free symbols 
	if(dict->symbols)
	{
		t_link *l;
		t_symbol *s;
		for(l=dict->symbols->first;l;l=l->next)
		{
			s = l->data;
			scene_delete(sc,s);
		}

		// free list
		scene_delete(sc,dict->symbols);
	}
}
