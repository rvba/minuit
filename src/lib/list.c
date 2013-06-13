/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "ctx.h"
#include "context.h"
#include "node.h"
#include "scene.h"
#include "event.h"
#include "term.h"
#include "list.h"
#include "brick.h"
#include "block.h"
#include "viewport.h"
#include "graph.h"


t_link *lst_link_get(t_lst *lst, int pos)
{
	if(lst->tot == 0)
		return NULL;

	int i = 0;
	t_link *link=lst->first;

	while(link)
	{
		if(i == pos)
		{
			return link;
		}
		link = link->next;
		i++;
	}

	return NULL;
}

void lst_link_swap(t_lst *lst, t_link *n1, t_link *n2)
{
	t_link *tmp = n1->prev;
	lst_link_remove(lst, n1);
	lst_link_insert(lst, n2->prev, n1);

	if(tmp != n2)
	{
		lst_link_remove(lst, n2);
		lst_link_insert(lst, tmp, n2);
	}
}
 
int get_pivot(int i,int j )
{
    return((i+j) /2);
}

inline int lst_link_get_order(t_link *link)
{
	return link->order;
}

int lst_sort(t_lst *lst,int m,int n)
{
    int key,i,j,k;
    if( m < n)
    {
        k = get_pivot(m,n);

	t_link *l1 = lst_link_get(lst,m);
	t_link *l2 = lst_link_get(lst,k);

	if(l1 && l2)
	{
        	lst_link_swap(lst,l1,l2);
	}
	else
	{
		if(!l1) printf("[ERR] no l1\n");
		if(!l2) printf("[ERR] no l2\n");
	}

        t_link *link_key = lst_link_get(lst,m);
	if(!link_key)
		printf("[ERR] No link key\n");

	key = link_key->order;

        i = m+1;
        j = n;

	int go_i = 1;
	int go_j = 1;

	while(i <= j)
	{
		//while((i <= n) && (lst_link_get_order(lst_link_get(lst,i)) <= key))
		while((i <= n) && go_i)
		{
			t_link *link = lst_link_get(lst,i);
			if(link)
			{
				int order = lst_link_get_order(link);
				if(order <= key)
					i++;
				else
					go_i = 0;
			}
			else
			{
				printf("[ERR] no link i\n");
				go_i = 0;
			}
		}

		while((j >= m) && go_j)
		{
			t_link *link = lst_link_get(lst,j);
			if(link)
			{
				int order = lst_link_get_order(link);
				if(order > key)
					j--;
				else
					go_j = 0;
			}
			else
			{
				printf("[ERR] no link j\n");
				go_j = 0;
			}

			if(i < j)
			{
				t_link *x = lst_link_get(lst,i);
				t_link *y = lst_link_get(lst,j);

				if(x && y)
				{
					lst_link_swap(lst,x,y);
				}
				else
				{
					if(!x) printf("[ERR] no x\n");
					if(!y) printf("[ERR] no y\n");
				}
			}
		}

		//  swap two elements 
		lst_link_swap(lst,lst_link_get(lst,m),lst_link_get(lst,j));

		// recursively sort the lesser list 
		lst_sort(lst,m,j-1);
		lst_sort(lst,j+1,n);
	}
    }

    return 1;
}

void link_show(const char *name,t_link *link)
{
	printf("%s\n",name);
	if(link)
	{
		printf("link %d\n",link->order);
		if(link->prev)
			printf("prev:%d\n",link->prev->order);
		else
			printf("prev:NULL\n");

		if(link->next)
			printf("next:%d\n",link->next->order);
		else
			printf("next:NULL\n");
	}
	else
	{
		printf("link is NULL\n");
	}
}
	

void lst_swap_simple(t_lst *lst, t_link *link)
{
	t_link *prev = link->prev;
	t_link *next = link->next;

	link->prev = prev->prev;

	if(link->prev)
	{
		link->prev->next = link;
	}
	else
	{
		lst->first = link; 
		link->prev=NULL;
	}

	link->next = prev;
	prev->prev = link;

	if(next)
	{
		prev->next = next;
		prev->next->prev = prev;
	}
	else
	{
		prev->next = NULL;
		lst->last = prev;
	}
}


int lst_sort_bubble(t_lst *lst)
{
	int swap=1;
	while(swap)
	{
		swap = 0;
		t_link *link;
		for(link=lst->first;link;link=link->next)
		{
			if(link->prev)
			{
				t_link *prev = link->prev;;
				int order_link = link->order;
				int order_prev = prev->order;

				if(order_link < order_prev)
				{
					lst_swap_simple(lst,link);
					swap = 1;
					break;
				}
			}
		}
	}
	return 1;
}

int lst_sort_quick(t_lst *lst)
{
	lst_sort(lst,0,lst->tot-1);
	return 1;
}

// add a new link to the end of the list
void lst_push_back(t_lst *lst,t_link *link)
{
	// next link
	if(lst->last)
	{
		link->prev=lst->last;
		lst->last->next=link;
		lst->last=link;
	}	
	// first link
	else
	{
		lst->first=link;
		lst->last=link;
	}

	lst->tot++;
}

void lst_link_insert(t_lst *lst, t_link *prev, t_link *link)
{
	if(link)
	{
		if(prev)
		{
			// Prev is in between
			if(prev->next)
			{
				link->prev = prev;
				link->next = prev->next;
				prev->next = link;
				link->next->prev  = link;
			}
			// Prev is Last
			else
			{
				prev->next = link;
				link->prev = prev;
				lst->last = link;
			}
		}
		// No Prev insert First
		else
		{
			if(lst->first)
			{
				link->next = lst->first;
				lst->first->prev = link;
				lst->first = link;
			}
			else
			{
				lst->first = link;
				lst->last = link;
			}
		}

		lst->tot++;
	}
}

// add an item to the list, make a new link, push it back
t_link *lst_add(t_lst *lst,void *data,const char *name)
{
	t_link *link=link_new(name);
	link->data=data;
	lst_push_back(lst,link);
	return link;
}

// merge two lists
void lst_add_lst(t_lst *dst, t_lst *src)
{
	t_link *link;
	for(link = src->first; link; link = link->next)
	{
		lst_add(dst, link->data, link->name);
	}
}
	

// show links in list
void lst_show(t_lst *lst)
{
	printf("lst_show %s\n",lst->name);
	printf("~\n");
	t_link *l;
	int i=0;
	//if(lst->first) printf("first: %d\n",lst->first->order);
	//if(lst->last) printf("last: %d\n",lst->last->order);
	for(l=lst->first;l;l=l->next)
	{
		//printf("%d %d %s\n",i,l->order,l->name);
		printf("%d %s\n",l->order,l->name);
		i++;
	}
	printf("~\n");
}

// remove a link from the list
void lst_link_remove(t_lst *lst,t_link *link)
{
	if(link)
	{
		// link is first
		if(!link->prev)
		{
			if(link->next)
			{
				lst->first=link->next;
				link->next->prev=NULL;
			}
			// unique link
			else
			{
				lst->first=NULL;
				lst->last=NULL;
			}
		}	
		// link is last 
		else if(!link->next)
		{
			// set new last
			link->prev->next=NULL;
			lst->last=link->prev;
		}
		// link is in between 
		else
		{
			// cut from the list
			link->next->prev=link->prev;
			link->prev->next=link->next;
		}

		// decrease tot
		lst->tot--;
	}

	link->next = NULL;
	link->prev = NULL;
}

// free a link
void link_free(t_link *link)
{
	free(link);
}

t_link *lst_link_find_by_name(t_lst *lst, const char *name)
{
	t_link *link;

	for(link = lst->first; link; link=link->next)
	{
		if(is(link->name,name))
			return link;
	}

	return NULL;
}

// remove a link from the list and free it
void lst_link_delete(t_lst *lst,t_link *link)
{
	lst_link_remove(lst,link);
	link_free(link);
}

void lst_link_delete_by_name(t_lst *lst, const char *name)
{
	t_link *link = lst_link_find_by_name(lst, name);

	if(link)
		lst_link_delete(lst, link);
	else
		printf("[ERROR lst_link_delete_by_name] Can't find link %s\n",name);
}

// remove and free all links from a list
void lst_delete_all(t_lst *lst)
{
	t_link *link=lst->first;
	t_link *tmp;

	while(link)
	{
		tmp=link;
		link=NULL;
		lst_link_delete(lst,tmp);
		lst_delete_all(lst);
	}
}

// remove all links
void lst_remove_all(t_lst *lst)
{
	t_link *link;

	if(lst->tot>0)
	{
		for(link=lst->first;link;link=link->next)
		{
			lst_link_remove(lst,link);
		}
	}
}

// remove and free all links
void lst_cleanup(t_lst *lst)
{
	t_link *link=lst->first;
	t_link *tmp=NULL;
	while(link)
	{
		tmp=link->next;
		lst_link_remove(lst,link);
		link_free(link);
		link=tmp;
	}
}

// free a list
void lst_free(t_lst *lst)
{
	lst_cleanup(lst);
	free(lst);
}

// copy a list
t_lst *lst_copy(t_lst *lst)
{
	if(lst->first)
	{
		t_lst *copy=lst_new(lst->name);
		t_link *link;

		for(link=lst->first;link;link=link->next)
		{
			void *data = link->data;
			lst_add(copy,data,link->name);
		}
		return copy;
	}
	else
	{
		return NULL;
	}
}

// link new
t_link *link_new(const char *name)
{
	t_link *link = (t_link *)malloc(sizeof(t_link));

	set_name(link->name,name);
	link->users=0;
	link->id=0;
	link->id_chunk=0;

	link->next=NULL;
	link->prev=NULL;
	link->parent=NULL;
	link->child=NULL;
	
	link->data=NULL;

	link->order = 0;

	return link;
}

// lst new
t_lst *lst_new(const char *name)
{
	t_lst *lst = (t_lst *)malloc(sizeof(t_lst));

	if(strlen(name)==0) 
		set_name(lst->name,"no_name");
	else
		set_name(lst->name,name);

	lst->id=0;
	lst->id_chunk=0;
	lst->users=0;
	lst->tot=0;
	lst->first=NULL;
	lst->last=NULL;
	lst->current=NULL;
	
	return lst;
}

// REMOVE DOUBLE

void lst_remove_double(t_lst *lst, t_link *current)
{
	t_link *link;
	t_link *link_to_remove = NULL;
	t_generic *generic = (t_generic *) current->data;
	int id = generic->id;

	for(link = current->next; link; link = link->next)
	{
		t_generic *g = (t_generic *) link->data;

		// remove previous stored link
		if(link_to_remove)
		{
			lst_link_delete(lst, link_to_remove);
			link_to_remove = NULL;
		}

		if(g->id == id)
		{
			// store link to remove
			link_to_remove = link;
		}
	}

	// case of last link double
	if(link_to_remove) lst_link_delete(lst, link_to_remove);
}

// REMOVE DOUBLES (generic ID)

void lst_remove_doubles(t_lst *lst)
{
	t_link *link;

	for(link = lst->first; link; link = link->next)
	{
		lst_remove_double(lst, link);
	}
}

void list_show(t_lst *lst)
{
	t_context *C = ctx_get();

	t_link *l;
	t_generic *g;

	if(C->event->debug_terminal)
	{
		printf("lst_show %s\n",lst->name);
		for(l=lst->first;l;l=l->next)
		{
			g = (t_generic *) l->data;
			printf("%s\n",g->name);
		}
	}

	if(C->event->debug_console)
	{
		term_log("lst_show %s\n",lst->name);
		for(l=lst->first;l;l=l->next)
		{
			g = (t_generic *) l->data;
			term_log("%s\n",g->name);
		}
	}
}

void lst_show_generic(t_lst *lst)
{
	printf("lst_show %s\n",lst->name);
	printf("~\n");
	t_link *l;
	for(l=lst->first;l;l=l->next)
	{
		t_generic *g = (t_generic *)l->data;
		printf("~ %s\n",g->name);
	}
	printf("~\n");
}

t_node *lst_get_node(t_lst *lst,int id)
{
	t_link *l;
	t_node *node;
//	assert(lst);

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

void *lst_get(t_lst *lst,char *name)
{
	t_link *l;
	t_generic *g;

	if(lst->first)
	{
		for(l=lst->first;l;l=l->next)
		{
			g=l->data;
			if(is(g->name,name))
			{
				return l->data;
			}
		}

		return NULL;
	}
	else
	{
		return NULL;
	}
}

// remove node
int lst_remove_node(t_lst *lst,t_node *node)
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
				lst_link_remove(lst,link); 
				return 1;
			}
		}

		// no match
		return 0;
	}
	return 0;
}

void lst_node_delete_all(t_lst *lst)
{
	t_link *l=lst->first;
	t_node *n;
	if(l)
	{
		for(;l;l=l->next)
		{
			n=l->data;
			n->cls->del(n);
		}
	}	

	lst_cleanup(lst);
}

// FIND NODE BY ID

t_link *lst_get_by_id(t_lst *lst,int id)
{
	t_link *l;
	t_generic *g;

	if(lst->first)
	{
		for(l=lst->first;l;l=l->next)
		{
			g=l->data;
			if(g->id==id)
			{
				return l;
			}
		}

		return NULL;
	}
	else
	{
		return NULL;
	}
}

int lst_remove_by_id(t_lst *lst,int id)
{
	t_link *link=lst_get_by_id(lst,id);

	if(link)
	{
		lst_link_delete(lst,link);
		return 1;
	}
	else
	{
		printf("[ERROR lst_remove_by_id] Can't get link\n");
		return 0;
	}
}

// FIND NODE BY NAME

t_node *lst_find_node_by_name(t_lst *lst,const char *name)
{
	t_link *link;
	t_node *node;
	t_generic *generic;

	if(lst->first)
	{
		for(link=lst->first;link;link=link->next)
		{
			node=link->data;
			generic=node->data;

			if(is(generic->name,name))
			{
				return node;
			}
		}

		printf("[ERROR lst_find_node by name] Can't find node:%s\n",name);
		return NULL;
	}
	else
	{
		//printf("[ERROR lst_find_node by name] Empty list\n");
		return NULL;
	}
}

t_link *list_add(t_lst *lst,void *data)
{
	t_context *C=ctx_get();

	t_generic *g = (t_generic *)data;

	t_node *node=scene_add(C->scene,nt_link,g->name);
	t_link *link=node->data;
	link->data=data;
	lst_push_back(lst,link);

	return link;
}

void list_remove(t_lst *lst);

void list_remove_by_name(t_lst *lst, const char *name)
{
	t_link *link;
	t_generic *g;

	for(link = lst->first; link; link = link->next)
	{
		g = (t_generic *) link->data;
		if(is (g->name, name))
		{
			lst_link_remove(lst,link);
		}
	}
}


void _link_free(t_link *link)
{
	free(link);
}

// CLONE

t_lst *lst_clone(t_lst *lst, t_data_type type)
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
				case(dt_graph):
					lst_add(clone,graph_clone(link->data),"graph");
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
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	t_link *l;

	for(l=lst->first;l;l=l->next)
	{
		scene_struct_delete(sc,l);
	}

	free(lst);
}

void _list_free(t_lst *lst, t_data_type type)
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
			case(dt_graph):
				_graph_free(link->data);
				break;
			default:
				printf("[WARNING _list_free] Unknown type %s\n",data_name_get(type));
				break;
		}
	}

	lst_cleanup(lst);
	free(lst);
}

void list_cleanup(t_lst *lst)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	t_link *l;

	for(l=lst->first;l;l=l->next)
	{
		scene_struct_delete(sc,l);
	}

}

void list_remove_by_ptr(t_lst *lst,void *ptr)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

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
		lst_link_remove(lst,link);
		scene_struct_delete(sc,link);
	}
	else
	{
		printf("[ERROR lst_remove_by_ptr] Can't find link\n");
	}
}

void list_remove_by_id(t_lst *lst, int id)
{
	t_context *C = ctx_get();
	t_scene *sc = C->scene;
	t_link *link;
	t_link *l;
	t_generic *g;

	for(l= lst->first; l; l = l->next)
	{
		g = (t_generic *) l->data;
		if(g->id == id)
		{
			link = l;
			break;
		}
	}

	if(link)
	{
		lst_link_remove(lst,link);
		scene_struct_delete(sc,link);
	}
	else
	{
		printf("[ERROR lst_remove_by_ptr] Can't find link\n");
	}
}

t_link *list_add_global(t_lst *lst,void *data)
{
	t_context *C=ctx_get();

	t_node *node=scene_add(C->scene,nt_link,"link");
	t_link *link=node->data;
	link->data=data;
	lst_push_back(lst,link);

	return link;
}

t_lst *lst_rebind(t_scene *sc,void *ptr)
{
	t_lst *lst=(t_lst *)ptr;

	rebind(sc,"list","first",(void **)&lst->first);
	rebind(sc,"list","last",(void **)&lst->last);
	rebind(sc,"list","current",(void **)&lst->current);

	return lst;
}

t_link *link_rebind(t_scene *sc,void *ptr)
{
	t_link *link=(t_link *)ptr;

	rebind(sc,"link","next",(void **)&link->next);
	rebind(sc,"link","prev",(void **)&link->prev);
	rebind(sc,"link",link->name,(void **)&link->data);

	link->parent=NULL;
	link->child=NULL;

	return link;
}




