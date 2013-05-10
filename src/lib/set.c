/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "op.h"

t_set *set_rebind(t_scene *sc, void **ptr)
{
	t_set *set=(t_set *)ptr;

	check_init("SET",set->name);
	rebind(sc,"set","lst",(void **)&set->lst);
	check_check("SET",set->name);

	return set;
}

t_node *set_add(const char *name)
{
	t_node *node_set = set_make(name);
	return node_set;
}

t_node *set_make(const char *name)
{
	t_context *C = ctx_get();
	t_node *node_set = scene_add(C->scene,nt_set,name);
	t_set *set = node_set->data;
	t_node *node_lst = scene_add(C->scene,nt_list,name);
	t_lst *lst = node_lst->data;
	set->lst = lst;
	return node_set;
};
	
t_set *set_new(const char *name)
{
	t_set *set = (t_set *)malloc(sizeof(t_set));

	set->id = 0;
	set->id_chunk = 0;
	set_name(set->name,name);
	set->users = 0;

	set->lst = NULL;

	return set;
}


