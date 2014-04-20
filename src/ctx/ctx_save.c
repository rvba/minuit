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

#include "sketch.h"
#include "mode.h"

void load_last( t_context *C)
{
	char *path = app_get_file_path( C->app, APP_FILENAME_SAVE);
	char *filename = NULL;
	t_file *file = NULL;

	if( sys_file_exists( path))
	{
		file = file_access( path);
	}

	if( file)
	{
		filename = file_line_get( file, 0);
		if( filename)
		{
			if( is( filename, "void")) filename = NULL;
		}
		else
		{
			printf("[LOAD] Can't get filename\n");
		}
	}

	if( filename)
	{
		load_file( C, filename);
		set_name_long( C->app->path_file, filename);
		t_file *f = file_new( filename);
		file_init( f);
		char new_name[_NAME_LONG_];
		bzero( new_name, _NAME_LONG_);
		set_name_long( new_name, f->file_name);
		s_cat( new_name, ".mn", _NAME_LONG_);
		set_name_long( C->app->filename, new_name);
		file_free( f);
	}
	else
	{
		C->event->callback = add_mn;
		screen_switch_by_name("screen_browser");
	}
}

void save_file(t_context *C)
{
	char path[_PATH_];
	char *filename = C->app->filename;

	if( !strlen( filename)) set_name_long( C->app->filename, "minuit.mn");

	s_cp( path, C->app->path_current, _PATH_);
	s_cat( path, "/" , _PATH_);
	s_cat( path, filename, _PATH_);
	s_cp( C->app->path_file, path, _PATH_);

	mem_write( path);
}

void mod_save( void)
{
	t_context *C = ctx_get();
	skt_save( C);
}

void save_to_file( t_context *C)
{
	t_link *l;
	t_node *node;

	option_save(C);
//	skt_save( C);
	
	t_module *module = mode_module_get( C->mode, "save");
	void (* f)( void) =  module->data;
	f();

	for(l=C->scene->nodes->first;l;l=l->next)
	{
		node = ( t_node *) l->data;
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
	char name[_NAME_LONG_];
	bzero( name, _NAME_LONG_);
	set_name_long( name, C->app->filename);

	t_file *file = file_new( name);
	file_init( file);

	s_cp( name, file->file_name, _NAME_LONG_);

	int length = strlen(name);
	int i,j;
	int last_char = length - 1; // starts from 0 !
	int max_indice = 10;
	char indice[max_indice];
	bzero(indice,max_indice);


	if( !isdigit( name[last_char]))
	{
		char number[4] = "-01";

		s_cat( name, number, _NAME_LONG_);
		s_cat( name, ".mn", _NAME_LONG_);
		set_name( C->app->filename, name);

	}
	else
	{
		j = 0;
		for(i = 0; i < length ; i++)
		{
			if( isdigit(name[i]))
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
				if( name[i] == '-')
				{
					break;
				}
				else
				{
					new_name[i] = name[i];
				}
			}

			s_cat( new_name, "-", _NAME_LONG_);
			s_cat( new_name, new_name_indice, _NAME_LONG_);
			s_cat( new_name, ".mn", _NAME_LONG_);
			set_name( C->app->filename, new_name);

		}
	}

	file_free( file);

	save_to_file(C);
}
