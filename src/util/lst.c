/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
 
// PUSH BACK 
// add a new link at the end of the list
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

	lst->count++;
}

// INSERT
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

		lst->count++;
	}
}

// REMOVE
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

		// decrease count
		lst->count--;
	}

	link->next = NULL;
	link->prev = NULL;

}

// SWAP
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

// ADD
t_link *lst_add(t_lst *lst,void *data,const char *name)
{
	t_link *link=link_new(name);
	link->data=data;
	lst_push_back(lst,link);
	return link;
}


t_link *lst_link_get(t_lst *lst, int pos)
{
	if(lst->count == 0)
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

// SORT

 
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
	lst_sort(lst,0,lst->count-1);
	return 1;
}

// merge two lists
void lst_add_lst(t_lst *dst, t_lst *src)
{
	t_link *link;
	for(link = src->first; link; link = link->next)
	{
		lst_add(dst, link->data, link->id.name);
	}
}

t_link *lst_link_find_by_name(t_lst *lst, const char *name)
{
	t_link *link;

	for(link = lst->first; link; link=link->next)
	{
		if(is(link->id.name, name))
		{
			return link;
		}
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

	if(lst->count>0)
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

// FIND NODE BY ID

t_link *lst_get_by_id(t_lst *lst,int item_id)
{
	t_link *l;
	t_id *id;

	if(lst->first)
	{
		for(l=lst->first;l;l=l->next)
		{
			id = (t_id *) l->data;
			if(id->id == item_id)
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

// copy a list
t_lst *lst_copy(t_lst *lst)
{
	if(lst->first)
	{
		t_lst *copy = lst_new(lst->id.name);
		t_link *link;

		for(link=lst->first;link;link=link->next)
		{
			void *data = link->data;
			lst_add(copy, data, link->id.name);
		}
		return copy;
	}
	else
	{
		return NULL;
	}
}


// REMOVE DOUBLE

void lst_remove_double(t_lst *lst, t_link *current)
{
	t_link *link;
	t_link *link_to_remove = NULL;

	t_id *id = (t_id *) current->data;

	for(link = current->next; link; link = link->next)
	{
		t_id *_id = (t_id *) link->data;

		// remove previous stored link
		if(link_to_remove)
		{
			lst_link_delete(lst, link_to_remove);
			link_to_remove = NULL;
		}

		if(_id->id == id->id)
		{
			// store link to remove
			link_to_remove = link;
		}
	}

	// case of last link double
	if(link_to_remove) lst_link_delete(lst, link_to_remove);
}

// REMOVE DOUBLES 

void lst_remove_doubles(t_lst *lst)
{
	t_link *link;

	for(link = lst->first; link; link = link->next)
	{
		lst_remove_double(lst, link);
	}
}

// SHOW

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

void lst_show(t_lst *lst)
{
	printf("lst_show %s\n", lst->id.name);
	printf("~\n");
	t_link *l;
	int i=0;
	for(l=lst->first;l;l=l->next)
	{
		printf("%d %s\n",l->order,l->id.name);
		i++;
	}
	printf("~\n");
}

// FREE

void link_free(t_link *link)
{
	mem_free( link, sizeof( t_link));
}

void lst_free(t_lst *lst)
{
	lst_cleanup(lst);
	mem_free( lst, sizeof( t_lst));
}


// NEW

t_link *link_new(const char *name)
{
	t_link *link = (t_link *)mem_malloc(sizeof(t_link));

	id_init(&link->id, name);

	link->next=NULL;
	link->prev=NULL;
	
	link->data=NULL;

	link->order = 0;

	return link;
}

t_lst *lst_new(const char *name)
{
	t_lst *lst = (t_lst *)mem_malloc(sizeof(t_lst));

	id_init(&lst->id, name);

	if(strlen(name)==0) 
	{
		set_name(lst->id.name, "no_name");
	}
	else
	{
		set_name(lst->id.name, name);
	}

	lst->count=0;
	lst->first=NULL;
	lst->last=NULL;
	lst->current=NULL;
	lst->type=dt_null;
	
	return lst;
}
