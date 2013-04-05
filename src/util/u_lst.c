/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"

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

// add an item to the list, make a new link, push it back
t_link *lst_add(t_lst *lst,void *data,const char *name)
{
	t_link *link=link_new(name);
	link->data=data;
	lst_push_back(lst,link);
	return link;
}

// show links in list
void lst_show(t_lst *lst)
{
	printf("lst_show %s\n",lst->name);
	printf("~\n");
	t_link *l;
	for(l=lst->first;l;l=l->next)
	{
		printf("~ %s\n",l->name);
	}
	printf("~\n");
}

// remove a link from the list
void lst_link_remove(t_lst *lst,t_link *link)
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
