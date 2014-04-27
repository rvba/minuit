/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef _DICT_H
#define _DICT_H

#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Context;
struct Scene;
struct Lst;
struct Node;

typedef struct Symbol
{
	t_id id;
	int id_chunk;
	short users;
	char name[_NAME_];

	enum Data_Type data_type;
	void *data;

}t_symbol;

typedef struct Dict
{
	t_id id;
	struct Lst *symbols;
	int count;

}t_dict;

t_symbol *		dict_symbol_add(t_dict *dict,const char *name,t_data_type type,void *data);
t_symbol *		dict_pop(t_dict *dict,const char *name);
void *			dict_pop_data(t_dict *dict,const char *name);
t_dict *		dict_make( const char *name);
void			symbol_free(t_symbol *symbol);
void			symbol_rebind(struct Scene *sc,void *ptr);
void *			symbol_new(const char *name);
void 			dict_free(t_dict *dict);
void			dict_rebind(struct Scene *sc,void *ptr);
void *			dict_new(const char *name);
void 			dict_show(t_dict *dict);

#ifdef __cplusplus
}
#endif

#endif
