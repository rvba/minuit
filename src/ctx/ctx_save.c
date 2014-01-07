/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "op.h"
#include "context.h"
#include "scene.h"
#include "app.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "file.h"
#include "util.h"
#include "list.h"
#include "brick.h"

void load_last(t_context *C)
{
	t_link *link;

	t_file *file = file_new("minuit.save");
	file_init(file);
	if(file_exists(file))
	{
		file_read(file);
		file_read_lines(file);
		if(file->lines->first)
		{
			link = file->lines->first;
			t_line *line = link->data;
			line_read_words(line);
			t_link *l = line->words->first;
			if(l)
			{
				t_word *word = l->data;
				char *name = word->data;
				if(is(name,"void"))
				{
					load_file(C,"minuit.mn");
				}
				else
				{
					load_file(C,word->data);
				}
			}
		}
	}
	else
	{
		C->event->callback=add_mn;
		screen_switch_by_name("screen_browser");
	}
}

void save_file(t_context *C)
{
	t_file *file = C->app->file;

	char *name = file->id.name;

	if(is(name,"void"))
	{
		set_name(file->id.name, "minuit.mn");
		file_build_location(file);
	}

	mem_write(file->location);
}

void save_to_file( t_context *C)
{
	t_link *l;
	t_node *node;

	option_save(C);

	for(l=C->scene->nodes->first;l;l=l->next)
	{
		node = l->data;
		if( node->store)
		{
			if( node->type==dt_var)
			{
				node->id_chunk_self = mem_store(ct_node,dt_var,sizeof(t_node),1,node);
				node->id_chunk = mem_store(ct_data,dt_var,node->size,1,node->id_ptr);
			}
			else
			{
				// store node && get chunk indice
				node->id_chunk_self = mem_store( ct_node, node->type, sizeof( t_node), 1, node);

				// store data && get chunk indice
				node->id_chunk = mem_store( ct_data, node->type, node->cls->size, 1, node->data);

				// copy chunk indice to generic
				t_id *id = (t_id *) node->data;
				id->id_chunk = node->id_chunk;
			}
		}
	}

	save_file( C);
}

void save_file_increment(t_context *C)
{
	t_file *file = C->app->file;

	file_path_split(file);

	int i,j;
	char *name = file->id.name;
	int length = strlen(name);
	int last_char = length - 1; // starts from 0 !
	int max_indice = 10;
	char indice[max_indice];
	bzero(indice,max_indice);

	if(!isdigit(name[last_char]))
	{
		char number[4] = "-01";

		strcat(file->id.name, number);
		set_name(file->id.name, name);
		file_build_location(file);
	}
	else
	{
		j = 0;
		for(i = 0; i < length ; i++)
		{
			if(isdigit(name[i]))
			{
				if(j < max_indice)
				{
					indice[j] = name[i];
					j++;
				}
			}

		}

		if(j)
		{
			int old_indice = atoi(indice);

			int new_indice = ++old_indice;

			char new_name[_PATH_];
			char new_name_indice[10];

			if(new_indice < 10)
			{
				sprintf(new_name_indice,"0%d",new_indice);
			}
			else
			{
				sprintf(new_name_indice,"%d",new_indice);
			}


			for(i = 0; i < length ; i++)
			{
				if(isdigit(name[i]))
				{
					break;
				}
				else
				{
					new_name[i] = name[i];
				}
			}

			strcat(new_name,new_name_indice);

			set_name(file->id.name, new_name);
			file_build_location(file);

		}
	}

	save_to_file(C);
}
