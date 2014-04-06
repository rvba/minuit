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
#define S_DEBUG 0

/* **MEM** **/


int file_exists(t_file *file)
{
	struct stat   buffer;   
	return (stat (file->location, &buffer) == 0);
}

void file_set_location(t_file *file, const char *path)
{
	set_path(file->location,path);
}

void file_go_backward(t_file *file)
{
	if(file->tot_directories>1)
	{
		file->tot_directories--;
		free(file->directories[file->tot_directories]);
		// change name
		set_name( file->id.name, file->directories[file->tot_directories-1]);
		file_build_location(file); 
	}
	else
	{
		printf("no backward possible\n");
	}
}

void file_element_add(t_file *file,char *name)
{
	file->directories=(char **)realloc(file->directories,sizeof(char *)*file->tot_directories+1);
	file->directories[file->tot_directories]=s_allocate(name);
	file->tot_directories++;
	// change name
	set_name( file->id.name, name);
}

void file_go_directory(t_file *file,char *name)
{
	file_element_add(file,name);
	file_build_location(file); 
}

// WORD

int word_equal(t_word *word,const char *string)
{
	if(strcmp(word->data,string)==0)
		return 1;
	else
		return 0;
}

void word_show(t_word *word)
{
	int i;
	printf("WORD:");
	for(i=0;i<word->size;i++)
	{
		putchar(word->data[i]);
	}
	putchar('\n');
}

t_word *word_new(void)
{
	t_word *word = (t_word *)malloc(sizeof(t_word));
	word->size=0;
	word->data=NULL;

	return word;
}

// LINE

void line_read_words(t_line *line)
{
	int tot_word=0;
	int i,j,k;

	//count words
	for(i=0;i<line->size;i++)
	{
		if(isspace(line->data[i]))
		{
			tot_word++;
		}
	}

	//count words size
	int size=0;
	int sizes[tot_word];
	j=0;

	for(i=0;i<line->size;i++)
	{
		if(isspace(line->data[i]))
		{
			sizes[j]=size+1;
			size=0;
			j++;
		}
		else
		{
			size++;
		}
	}
	
	j=0;
	k=0;
	t_word *word=word_new();
	word->size=sizes[j];
	word->data=(char *)malloc(sizeof(char)*sizes[j]);

	for(i=0;i<line->size;i++)
	{
		if(isspace(line->data[i]))
		{
			word->data[k]='\0';
			lst_add(line->words,word,"word");

			if(i<line->size)
			{
				k=0;
				j++;

				word=word_new();
				word->size=sizes[j];
				word->data=(char *)malloc(sizeof(char)*sizes[j]);
			}
		}
		else
		{
			word->data[k]=line->data[i];
			k++;
		}
	}
}

void line_show(t_line *line)
{
	int i;
	printf("LINE %d:",line->size);
	for(i=0;i<line->size;i++)
	{
		putchar(line->data[i]);
	}

	if(line->words)
	{
		t_link *link;
		for(link=line->words->first;link;link=link->next)
		{
			t_word *word = link->data;
			word_show(word);
		}
	}
}

t_line *line_new(void)
{
	t_line *line=(t_line *)malloc(sizeof(t_line));
	line->size=0;
	line->data=NULL;
	line->words=lst_new("lst");

	return line;
}

// FILE

/** store lines without \n **/
int file_read_lines(t_file *file)
{
	//if(file->data_size>0)
	if( file->data)
	{

		file->lines = lst_new("lst");

		// count lines
		int i,j,k;
		int totline=0;
		int tot=0;
		for(i=0;i<file->data_size;i++)
		{
			if(file->data[i]=='\n')
			{	
				totline++;
			}
		}

		// count line size

		int line_size[totline];
		j=0;

		for(i=0;i<file->data_size;i++)
		{
			if(file->data[i]=='\n')
			{
				tot++;
				line_size[j]=tot;
				tot=0;
				j++;
			}
			else
			{
				tot++;
			}
		}

		// store lines

		j=0;
		k=0;

		t_line *line=line_new();
		line->size=line_size[j];
		line->data=(char *)malloc(sizeof(char)*line_size[j]);

		for(i=0;i<file->data_size;i++)
		{
			if(file->data[i]=='\n')
			{
				//line->data[k]='\n';
				line->data[k]='\0';
				lst_add(file->lines,line,"line");

				j++;
				k=0;

				if(j<totline)
				{
					int size = line_size[j];
					line=line_new();
					line->size=size;
					line->data=(char *)malloc(sizeof(char)*size);
				}
			}
			else
			{
				line->data[k]=file->data[i];
				k++;
			}

		}

		return 1;
	}
	else
	{
		printf("[FILE] Error, no data to read\n");
		return 0;
	}
}

char *file_line_get( t_file *file, int p)
{
	if( file->data)
	{
		if( file->lines)
		{
			t_line *line = ( t_line *) lst_get_by_range( file->lines, p);
			if( line) 
			{
				return line->data;
			}
			else
			{
				printf("[FILE] Error, Can't get %d line\n", p);
				return NULL;
			}
		}
		else
		{
			if( file_read_lines( file))
			{
				return file_line_get( file, p);
			}
			else
			{
				return NULL;
			}
		}
	}
	else
	{
		if( file_read( file))
		{
			return file_line_get( file, p);
		}
		else
		{
			return NULL;
		}
	}
}

void file_show(t_file *file)
{
	printf("FILE\n");
	if(file->location) printf("location:%s\n",file->location);
	if(file->path) printf("path:%s",file->path);
	if( file->path_type == PATH_RELATIVE) printf(", path is relative\n");
	else if( file->path_type == PATH_ABSOLUTE) printf(", path is absolute\n");
	else if( file->path_type == PATH_LOCAL) printf(", path is local\n");
	else printf(", path of unknown type\n");
	if(file->id.name) printf("name:%s\n", file->id.name);
	if(file->ext) printf("ext:%s\n",file->ext);
	printf("tot_directories:%d\n",file->tot_directories);
	if(file->tot_directories)
	{
		int i;
		for(i=0;i<file->tot_directories;i++)
		{
			printf("elem %d:%s\n",i,file->directories[i]);
		}
	}
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


// LOCATION

void file_build_location(t_file *file)
{
	int i;
	int cursor=0;

	char new_path[PATH_LIMIT];
	bzero(new_path,PATH_LIMIT);

	// relative path
	if(file->is_relative)
	{
		new_path[0] = '.';
		cursor++;
	}

	// loop over directories "/" 
	for(i=0;i<file->tot_directories -1 ;i++)
	{
		// get directory name
		char *add=file->directories[i];

		new_path[cursor]='/';
		cursor++;
		while(*add!='\0')
		{
			new_path[cursor]=*add;
			add++;
			cursor++;
		}
	}


	new_path[cursor]='/';
	strcat( new_path, file->id.name);

	if(file->has_extention)
	{
		strcat(new_path,".");
		strcat(new_path,file->ext);
	}

	set_path(file->location,new_path);
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

int _file_test( const char *path)
{
	printf("TESTING %s\n", path);
	t_file *file = file_new( "test");
	set_path( file->path, path);

	int s = 1;
	file_path_type( file);
	if( s) s = file_dir_build( file);
	if( s) s = file_name_build( file);

	file_show( file);
	file_free( file);

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

int file_test( void)
{
	_file_test( "test");
	_file_test( "/home/user/Desktop/");
	_file_test( "./data");
	_file_test( "/usr/share/image.jpg");
	return 1;
}

// REBIND

t_file *file_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}

// FREE

void file_free(t_file *file)
{
	if(file->directories)
	{
		int i;

		for(i=0;i<file->tot_directories;i++)
		{
			free(file->directories[i]);
		}

		free(file->directories);
	}
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

t_file *file_new(const char *path)
{
	t_file *file = (t_file *) malloc( sizeof( t_file));

	id_init( &file->id, path);

	memset( file->id.name,'\0',_NAME_);
	memset( file->ext,'\0',_EXT_);
	memset( file->path,'\0',_PATH_);

	set_path( file->location,path);
	set_path( file->path, path);

	file->is_relative = 0;
	file->is_directory = 0;
	file->has_extention = 0;
	file->path_type = 0;

	file->directories = NULL;
	file->data = NULL;
	file->lines = NULL;
	file->file = NULL;

	file->tot_directories = 0;
	file->data_size = 0;
	file->tot_line = 0;
	file->dir_count = 0;
	file->exists = 0;

	bzero( file->dirs, FILE_MAX_DIR * _NAME_LONG_);
	bzero( file->dir_path, _PATH_);

	return file;
}



