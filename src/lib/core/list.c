/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "op.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "event.h"
#include "term.h"
#include "list.h"
#include "brick.h"
#include "block.h"
#include "viewport.h"
#include "rhizome.h"
#include "memory.h"


void list_show(t_lst *lst)
{
	t_context *C = ctx_get();

	t_link *l;
	t_id *id;

	if(C->event->debug_terminal)
	{
		printf("lst_show %s\n", lst->id.name);
		for(l=lst->first;l;l=l->next)
		{
			id = (t_id *) l->data;
			printf("%s\n",id->name);
		}
	}

	if(C->event->debug_console)
	{
		term_log("lst_show %s\n",lst->id.name);
		for(l=lst->first;l;l=l->next)
		{
			id = (t_id *) l->data;
			term_log("%s\n",id->name);
		}
	}
}

t_node *list_node_exists(t_lst *lst,int id)
{
	t_link *l;
	t_node *node;

	for(l=lst->first;l;l=l->next)
	{
		node=l->data;
		if(node->id==id)
		{
			return node;
		}
	}

	return NULL;
}

// remove node
int list_remove_node(t_lst *lst,t_node *node)
{
	t_link *link;
	if(lst->first)
	{
		for(link=lst->first;link;link=link->next)
		{
			t_node *n = link->data;
			//match node
			if(node->id==n->id)
			{
				list_link_remove(lst,link); 
				return 1;
			}
		}

		// no match
		return 0;
	}
	return 0;
}

// FIND NODE BY NAME

t_node *list_find_node_by_name(t_lst *lst,const char *name)
{
	t_link *link;
	t_node *node;
	t_id *id;

	if(lst->first)
	{
		for(link=lst->first;link;link=link->next)
		{
			node=link->data;
			id = (t_id *) node->data;

			if(iseq(id->name,name))
			{
				return node;
			}
		}

//		printf("[ERROR list_find_node by name] Can't find node:%s\n",name);
		return NULL;
	}
	else
	{
//		printf("[ERROR lst_find_node by name] Empty list\n");
		return NULL;
	}
}

t_node *list_find_node_by_id( t_lst *lst, int id)
{
	t_link *l;
	t_node *node;

	for(l=lst->first;l;l=l->next)
	{
		node = l->data;
		if( node->id == id)
		{
			return node;
		}
	}

	printf("[ERROR list_find_node_by_id] Node not found\n");
	return NULL;
}

t_link *list_add_data( t_lst *lst, void *data)
{
	t_context *C=ctx_get();

	t_id *id = (t_id *) data;
	t_link *link;

	t_node *node=scene_add(C->scene,dt_link,id->name);
	link=node->data;

	link->data=data;
	lst_push_back(lst,link);

	return link;
}

void list_remove_by_name(t_lst *lst, const char *name)
{
	t_link *link;
	t_id *id;

	for(link = lst->first; link; link = link->next)
	{
		id = (t_id *) link->data;
		if(iseq(id->name, name))
		{
			list_link_remove(lst,link);
		}
	}
}


void _link_free(t_link *link)
{
	// link is remove removed from scene by list_free
	mem_free( link, sizeof( t_link));
}

// CLONE

t_lst *list_clone(t_lst *lst, t_data_type type)
{
	if(lst)
	{
		t_lst *clone = lst_new("clone");
		t_link *link;
		for(link=lst->first;link;link=link->next)
		{
			switch(type)
			{
				case(dt_block):
					lst_add(clone,block_clone(link->data),"block");
					break;
				case(dt_brick):
					lst_add(clone,brick_clone(link->data),"brick");
					break;
				case(dt_viewport):
					lst_add(clone,viewport_clone(link->data),"viewport");
					break;
				case(dt_rhizome):
					lst_add(clone,rhizome_clone(link->data),"rhizome");
					break;
				default:
					printf("[lst_clone WARNING] Unknown type %s\n",data_name_get(type));
					break;
			}
		}
		//lst_show(clone);

		return clone;
	}
	else
	{
		return NULL;
	}
}


// free links && list 
// but not the data it points to
void list_free(t_lst *lst)
{
	if( lst->id.id)
	{
		list_cleanup( lst);
		mem_free( lst, sizeof( t_lst));
	}
	else
	{
		printf("[WARINING] list_free: list not in scene\n");
		lst_free( lst);
	}
}

void list_free_data(t_lst *lst, t_data_type type)
{
	t_link *link;

	for(link=lst->first;link;link=link->next)
	{
		switch(type)
		{
			case(dt_block):
				_block_free(link->data);
				break;
			case(dt_brick):
				_brick_free(link->data);
				break;
			case(dt_viewport):
				_viewport_free(link->data);
				break;
			case(dt_rhizome):
				_rhizome_free(link->data);
				break;
			default:
				printf("[WARNING list_free_data] Unknown type %s\n",data_name_get(type));
				break;
		}
	}

	lst_cleanup(lst);
	mem_free( lst, sizeof( t_lst));
}

void list_cleanup(t_lst *lst)
{
	if( lst->id.id)
	{
		t_link *link=lst->first;
		t_link *tmp=NULL;
		while(link)
		{
			tmp=link->next;
			list_link_remove(lst,link);
			link=tmp;
		}
	}
	else
	{
		printf("[WARINING] list_cleanup: list not in scene\n");
		lst_cleanup( lst);
	}
}

void list_link_remove( t_lst *lst, t_link *link)
{
	t_scene *sc = scene_get();
	lst_link_remove( lst, link);
	scene_delete( sc, link);
}

void list_remove_by_ptr(t_lst *lst,void *ptr)
{
	t_link *l;
	t_link *link=NULL;

	for(l=lst->first;l;l=l->next)
	{
		if(l->data==ptr)
		{
			link=l;
			break;
		}
	}

	if(link)
	{
		list_link_remove(lst,link);
	}
	else
	{
		printf("[ERROR lst_remove_by_ptr] Can't find link\n");
	}
}

void list_remove_by_id(t_lst *lst, int item_id)
{
	t_link *link = NULL;
	t_link *l;
	t_id *id;

	for(l= lst->first; l; l = l->next)
	{
		id = (t_id *) l->data;
		if(id->id == item_id)
		{
			link = l;
			break;
		}
	}

	if(link)
	{
		list_link_remove(lst,link);
	}
	else
	{
		printf("[ERROR lst_remove_by_ptr] Can't find link\n");
	}
}

// REBIND

void list_rebind(t_scene *sc,void *ptr)
{
	t_lst *lst=(t_lst *)ptr;

	rebind(sc,"list","first",(void **)&lst->first);
	rebind(sc,"list","last",(void **)&lst->last);
	rebind(sc,"list","current",(void **)&lst->current);

}

void link_rebind(t_scene *sc,void *ptr)
{
	t_link *link=(t_link *)ptr;


	rebind(sc,"link","next",(void **)&link->next);
	rebind(sc,"link","prev",(void **)&link->prev);
	rebind(sc,"link", link->id.name, (void **)&link->data);
}

t_lst *list_make( t_data_type type, const char *name)
{
	t_scene *sc = ctx_scene_get();
	t_lst *lst;
	t_node *node = scene_add( sc, dt_list, name);
	lst = node->data;

	return lst;
}

void cls_list_delete( void *data)
{
	t_lst *lst = (t_lst *) data;
	list_free( lst);
}




