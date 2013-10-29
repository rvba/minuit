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

#define PATH_LIMIT 1024
#define S_DEBUG 0



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
void file_read_lines(t_file *file)
{
	if(file->data_size>0)
	{
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
				line->data[k]='\n';
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
	}
}

void file_show(t_file *file)
{
	printf("SHOW FILE\n");
	if(file->location) printf("location:%s\n",file->location);
	if(file->path) printf("path:%s",file->path);
	if(file->is_relative) printf("(path is relative)\n");
	else printf("(file is absolute)\n");
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

void file_open(t_file *file)
{
	if(!file->file)
	{
		file->file=fopen(file->location,"w");
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

void file_read(t_file *file)
{
	FILE *f = fopen(file->location,"r");
	fseek (f,0,SEEK_END);
	file->data_size = ftell(f);
	rewind(f);
	file->data = (char *)malloc(sizeof(char)*file->data_size);
	size_t r = fread (file->data,1,file->data_size,f);
	if(r != file->data_size) printf("read error\n");

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

int file_path_split(t_file *file)
{
	int i,j;
	char *letter;
	int slash_count=0;
	int *size;
	int indice=-1;
	int start=0;

	char *path = file->location;

	// PATH STARTS WITH DOT > go to first slash ./
	if (path[0] == '.')
	{
		path++;
		file->is_relative = 1;
	}
	else
	{
		file->is_relative = 0;
	}

	// COUNT SLASHES
	for(letter=path;*letter!='\0';letter++)
	{
		if(*letter == '/') slash_count++;
	}

	// SLASH iS 0 
	if(slash_count==0)
	{
		file->tot_directories=0;
		file->is_relative = 1;

		return 0;
	}

	// BUILD directories
	else
	{
		// size: length of directories
		size = (int *) malloc(sizeof(int) * slash_count);

		// check size of string directories
		for(letter=path; *letter!='\0'; letter++)
		{
			if(*letter == '/')
			{
				// go to next indice
				indice++;
				// reset size
				size[indice]=0;
			}
			else
			{
				// increment size
				size[indice]++;
			}
		}

		// directories
		file->directories = (char**) malloc( sizeof(char *) * slash_count);

		// allocate parts
		start = 0;

		for(i=0; i < slash_count; i++)
		{
			//first slash
			start++;

			//XXX add 2 more char : start from 0 +  one for endline
			file->directories[i] = (char *)malloc(sizeof(char) * (size[i]+1));

			for(j=0;j<size[i];j++)
			{
				file->directories[i][j] = path[start];
				start++;
			}

			file->directories[i][j] = '\0';

			if(S_DEBUG) printf("element:%s\n",file->directories[i]);
		}

		file->tot_directories = slash_count;

		// free
		free(size);

		return 1;
	}
}

// REBIND

t_file *file_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}

// INIT

int file_init(t_file *file)
{
	int i,j;
	char *letter;
	char *filename;
	int indice=-1;
	int is_split;
	int with_extension=0;

	// if no location > return
	if(strlen(file->location)==0)
	{
		printf("null path\n");
		return 0;
	}

	// split by /
	is_split=file_path_split(file);

	// get filename
	if(is_split)
	{
		filename=file->directories[file->tot_directories-1];
	}
	else
	{
		filename=file->location;
	}

	// reset indice
	indice = 0;

	// check if name has extention
	for(letter=filename;*letter!='\0';letter++)
	{
		if(*letter == '.')
		{
			with_extension=1;
			file->has_extention = 1;
			break;
		}
		else
		{
			indice++;
		}
	}

	// set name && extention
	if(with_extension)
	{
		// set name
		i=0;

		for(letter=filename;*letter!='.';letter++)
		{
			file->id.name[i] = *letter;
			i++;
		}

		file->id.name[i] = '\0';

		// set extention
		i++;
		j=0;

		letter=filename;
		letter+=i;

		for(;*letter!='\0';letter++)
		{
			file->ext[j]=*letter;
			j++;
		}

		file->ext[j] = '\0';
	}
	else
	{
		set_name( file->id.name, file->location);
	}


	return 1;
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

// NEW

t_file *file_new(const char *path)
{
	t_file *file = (t_file *)malloc(sizeof(t_file));

	id_init( &file->id, path);

	memset(file->id.name,'\0',_NAME_);
	memset(file->ext,'\0',_EXT_);
	memset(file->path,'\0',_PATH_);

	set_path(file->location,path);

	file->is_relative = 0;
	file->is_directory = 0;
	file->has_extention = 0;

	file->directories=NULL;
	file->data=NULL;
	file->lines=lst_new("lst");
	file->file=NULL;

	file->tot_directories=0;
	file->data_size=0;
	file->tot_line=0;

	return file;
}



