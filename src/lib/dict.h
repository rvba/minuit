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

struct Scene;
struct Lst;
struct Node;

typedef struct Symbol
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	enum Data_Type data_type;
	void *data;

}t_symbol;

typedef struct Dict
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	struct Lst *symbols;
	int count;

}t_dict;

t_symbol *		dict_symbol_add(t_dict *dict,const char *name,t_data_type type,void *data);
t_symbol *		dict_pop(t_dict *dict,const char *name);
void *			dict_pop_data(t_dict *dict,const char *name);
struct Node *		dict_add(const char *name);
void			symbol_free(t_symbol *symbol);
t_symbol *		symbol_rebind(struct Scene *sc,void *ptr);
t_symbol *		symbol_new(const char *name);
void 			dict_free(t_dict *dict);
t_dict *		dict_rebind(struct Scene *sc,void *ptr);
t_dict *		dict_new(const char *name);
void 			dict_show(t_dict *dict);


#endif
