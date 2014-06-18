/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include <sys/stat.h>
#include <sys/resource.h>

#include "util.h"
#include "scene.h"
#include "node.h"
#include "file.h"
#include "list.h"
#include "memory.h"

#define PATH_LIMIT 1024

int file_exists(t_file *file)
{
	struct stat   buffer;   
	return (stat (file->path, &buffer) == 0);
}

void file_go_backward( t_file *file)
{
	if( file->dir_count > 0)
	{
		file->dir_count--;
		file_path_build( file);
	}
}

void file_go_directory( t_file *file, char *name)
{
	file->dir_count++;
	s_cp( file->dirs[ file->dir_count], name, _NAME_LONG_);
	file_path_build( file);
}

void file_name_add( t_file *file, char *name)
{
	file_path_build( file);
	int l = strlen( file->path);
	s_append( file->path, name, l);
}

void file_show(t_file *file)
{
	printf("FILE\n");
	if(file->path) printf("path:%s",file->path);
	if( file->path_type == PATH_RELATIVE) printf(", path is relative\n");
	else if( file->path_type == PATH_ABSOLUTE) printf(", path is absolute\n");
	else if( file->path_type == PATH_LOCAL) printf(", path is local\n");
	else printf(", path of unknown type\n");
	if(file->id.name) printf("name:%s\n", file->id.name);
	if(file->ext) printf("ext:%s\n",file->ext);
	if(file->data)
	{
		int i;
		for(i=0;i<file->data_size;i++)
		{
			printf("%c",file->data[i]);
		}
	}

	printf("DIRS\n");
	printf(" dir count: %d\n", file->dir_count);
	int i;
	for( i= 0; i < FILE_MAX_DIR; i++)
	{
		printf("%d: %s\n", i, file->dirs[i]);
	}
	printf("NAME\n");
	printf("file name:%s\n", file->file_name);
	if( file->has_extention) printf("ext:%s\n", file->ext);
	else printf("ext:NULL\n");
}

void file_data_add(t_file *file,char *data)
{
	if(file->data)
	{
		free(file->data);
	}

	file->data=(char *)malloc(sizeof(char)*(strlen(data)+1));
	strcpy(file->data,data);
}

int file_open( t_file *file)
{
	if( file->file)
	{
		printf("[FILE] Error, file yet open\n");
		return 1;
	}
	else
	{
		if( sys_file_exists( file->path))
		{
			file->file = fopen( file->path, "w");
			return 1;
		}
		else
		{
			printf("[FILE] Error, file doesn't exists: %s\n", file->path);
			return 0;
		}
	}
}

void file_close(t_file *file)
{
	if(file->file)
	{
		fclose(file->file);
	}
	else
	{
		printf("no file to close!\n");
	}
}

int file_read( t_file *file)
{
	FILE *f = fopen( file->path, "r");

	if( f)
	{
		fseek ( f, 0, SEEK_END);
		file->data_size = ftell( f);
		rewind( f);
		file->data = (char *) malloc( sizeof( char) * file->data_size);
		size_t r = fread ( file->data, 1, file->data_size, f);
		if( r != file->data_size) printf("read error\n");

		return 1;
	}
	else
	{
		printf("[FILE] Error, Can't open %s\n", file->path);
		return 0;
	}
}

int file_write( t_file *file, const char *data, int size)
{
	if( file_open( file))
	{
		fwrite( data, sizeof(char) * size, 1, file->file);
		file_close( file);
		return 1;
	}
	else
	{
		return 0;
	}
}

// SPLIT
// search for path / name / extention
// WARNING: case path end with slash not implemented
// WARNING: case of dotted file (hidden) not implemented

void file_path_type( t_file *file)
{
	if( file->path[0] == '.') file->path_type = PATH_RELATIVE;
	else if( file->path[0] == '/') file->path_type = PATH_ABSOLUTE;
	else file->path_type = PATH_LOCAL;
}

int file_slash_count( t_file *file)
{
	char *letter;
	int slash_count = 0;
	for( letter = file->path; *letter!='\0'; letter++)
	{
		if( *letter == '/') slash_count++;
	}

	return slash_count;
}

void file_path_build( t_file *file)
{
	int c = 0;
	char path[_PATH_];
	bzero( path, _PATH_);
	if( file->path_type == PATH_RELATIVE)
	{
		path[0] = '.';
		c++;
	}
	else if( file->path_type == PATH_ABSOLUTE)
	{
		path[0] = '/';
		c++;
	}

	int i;
	for( i = 0; i <= file->dir_count; i++)
	{
		c = s_append( path, file->dirs[i], c);
		c = s_append( path, "/", c);
	}

	s_cp( file->path, path, _PATH_);
}

int file_dir_build( t_file *file)
{
	int slash_count = file_slash_count( file);
	int i = 0;
	int j = 0;
	if( slash_count < FILE_MAX_DIR)
	{
		char *letter = file->path;
		if( file->path_type == PATH_RELATIVE) letter += 2;
		else if( file->path_type == PATH_ABSOLUTE) letter++;

		for( ; *letter != '\0'; letter++)
		{
			if( *letter == '/')
			{
				file->dirs[i][j + 1] = '\0';
				i++;
				j = 0;
			}
			else
			{
				if( j >= _NAME_LONG_) 
				{
					printf("[FILE] Error, name too long\n");
					return 0;
				}
				file->dirs[i][j] = *letter;
				j++;
			}
		}

		return 1;
	}
	else
	{
		printf("[FILE] Error, too much directories: %d\n", slash_count);
		return 0;
	}
}

void file_extention_get( t_file *file)
{
	char *letter;
	char *data = file->dirs[ file->dir_count];
	char name[_NAME_LONG_];
	char ext[_EXT_];
	int has_extention = 0;
	bzero( name, _NAME_LONG_);
	bzero( ext, _EXT_);

	int i = 0;
	int j = 0;
	int ext_length = 0;

	for( letter = data; *letter != '\0'; letter++)
	{
		if( i >= _NAME_LONG_)
		{
			printf("[FILE] Error name too long\n");
			return;
		}
		else
		{
			if( *letter == '.')
			{
				name[j] ='\0';
				has_extention = 1;
				j = -1;
			}
			else
			{
				if( has_extention)
				{
					ext[j] = *letter;
					ext_length++;
				}
				else
				{
					name[j] = *letter;
				}
			}
		}

		i++;
		j++;
	}

	s_cp( file->file_name, name, _NAME_LONG_);

	if( has_extention)
	{
		ext[ext_length] = '\0';
		file->has_extention = 1;
		s_cp( file->ext, ext, _EXT_);
	}
}

void file_dir_count( t_file *file)
{
	int i;
	int count = 0; 
	for( i = 0; i < FILE_MAX_DIR; i++)
	{
		if( strlen( file->dirs[i]) == 0) break;
		else count++;
	}

	file->dir_count = count - 1;
}

int file_name_build( t_file *file)
{
	file_dir_count( file);
	file_extention_get( file);

	return 1;
}

int file_init( t_file *file)
{
	int s = 1;
	file_path_type( file);
	if( s) s = file_dir_build( file);
	if( s) s = file_name_build( file);

	return s;
}

// REBIND

void file_rebind(t_scene *scene, void *ptr)
{
}

// FREE

void file_free(t_file *file)
{
}

// ACCESS

t_file *file_access( const char *path)
{
	if( sys_file_exists( path))
	{
		t_file *file = file_new( path);
		return file;

	}
	else
	{
		printf("[FILE] Error, can't access %s\n", path);
		return NULL;
	}
}

// CREATE

int file_create( const char *path)
{
	t_file *file = file_new( path);
	if ( file_init( file))
	{
		file->file = fopen( file->path, "w");
		if( file->file)
		{
			file_close( file);
			file_free( file);
			return 1;
		}
		else
		{
			printf("[FILE] Error can't create %s\n,", path);
			file_free( file);
			return 0;
		}
	}
	else
	{
		file_free( file);
		return 0;
	}
}

// NEW

void *file_new(const char *path)
{
	t_file *file = (t_file *) malloc( sizeof( t_file));

	id_init( &file->id, path);

	memset( file->id.name,'\0',_NAME_);
	memset( file->ext,'\0',_EXT_);
	memset( file->path,'\0',_PATH_);

	set_path( file->path, path);

	file->is_relative = 0;
	file->is_directory = 0;
	file->has_extention = 0;
	file->path_type = 0;

	file->data = NULL;
	file->lines = NULL;
	file->file = NULL;

	file->data_size = 0;
	file->tot_line = 0;
	file->dir_count = 0;
	file->exists = 0;

	bzero( file->dirs, FILE_MAX_DIR * _NAME_LONG_);
	bzero( file->dir_path, _PATH_);

	return file;
}



