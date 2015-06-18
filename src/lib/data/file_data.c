/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "scene.h"
#include "node.h"
#include "file.h"
#include "list.h"
#include "memory.h"

void line_free( t_line *line);

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

void line_add_data( t_line *line, int pos, int size, char *data)
{
	int new_size = line->size + size;
	char *_new_data = ( char *) malloc( sizeof(char) * new_size);
	char *new_data = _new_data;
	char *old_data = line->data;
	int i;
	int length = 0;
	int add = 0;
	for( i = 0; i < new_size; i++)
	{
		if( i == pos) add = 1;
		if( add)
		{
			*new_data = *data;
			new_data++;
			data++;
			length++;
			if( length == size) add = 0;
		}
		else
		{
			*new_data = *old_data;
			new_data++;
			old_data++;
		}
	}

	line->size = new_size;
	free( line->data);
	line->data = _new_data;
}

void line_remove( t_file *file, int pos)
{
	t_line *line = file_line_get( file, pos);
	t_link *link = lst_link_get( file->lines, pos);
	lst_link_remove( file->lines, link);
	file->tot_line--;
	line_free( line);
}

void line_remove_data( t_line *line, int pos, int size)
{
	int old_size = line->size;
	if( old_size > 1)
	{
		int new_size = line->size - size;
		char *_new_data = ( char *) malloc( sizeof(char) * new_size);
		char *new_data = _new_data;
		char *old_data = line->data;
		int i;
		int length = 0;
		int remove = 0;
		for( i = 0; i < old_size; i++)
		{
			if( i == pos) remove = 1;
			if( remove)
			{
				old_data++;
				length++;
				if( length == size) remove = 0;
			}
			else
			{
				*new_data = *old_data;
				new_data++;
				old_data++;
			}
		}

		line->size = new_size;
		free( line->data);
		line->data = _new_data;
	}
}


// cut line after char_pos
void line_cut( t_line *line, int char_pos)
{
	int new_size = char_pos + 1; // \0
	size_t new_data_size = sizeof(char) * new_size;
	char *old_data = line->data;
	char *new_data = ( char *) realloc( old_data, new_data_size);
	new_data[char_pos] = '\0';
	line->size = new_size;
	line->data = new_data;
}

// make new line with (split) data after line_pos
t_line *line_split_make( t_file *file, int line_pos,  const char *data)
{
	// new line
	t_line *line = line_new();

	// new data
	int size = strlen( data) + 1;
	char *new_data = ( char *) malloc( sizeof(char) * size);

	// copy data
	memcpy( new_data, data, size);

	// set line param
	line->data = new_data;
	line->size = size;

	// insert in file
	file->tot_line++;
	t_link *l = lst_link_get( file->lines, line_pos);
	lst_insert( file->lines, l, line, "line");

	return line;
}

// split line at char_pos
t_line *line_split( t_file *file, int line_pos, int char_pos)
{
	// get current line
	t_line *line = file_line_get( file, line_pos);

	// get pointer to split point
	char *data = line->data + char_pos;

	// create split line
	t_line *l = line_split_make( file, line_pos, data);

	// cut current line
	line_cut(line, char_pos);

	return l;
}
t_line *file_line_join( t_file *file, int before, int after)
{
	t_line *line_before = file_line_get( file, before);
	t_line *line_after = file_line_get( file, after);
	int pos = line_before->size-1; //starts at /0
	int size = line_after->size;
	char *data = line_after->data;
	line_add_data( line_before, pos, size, data);
	line_remove( file, after);
	line_before->size--;
	return line_before;
}

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
	printf("LINE (size%d):",line->size);
	for(i=0;i<line->size;i++)
	{
		putchar(line->data[i]);
	}

	printf("\n");

	if(line->words)
	{
		t_link *link;
		for(link=line->words->first;link;link=link->next)
		{
			t_word *word = link->data;
			word_show(word);
		}
	}

	printf("\n");
}

void *line_new(void)
{
	t_line *line=(t_line *)malloc(sizeof(t_line));
	line->size=0;
	line->data=NULL;
	line->words=lst_new("lst");

	return line;
}

void word_free( t_word *word)
{
	if( word->data) free( word->data);
}

void line_free( t_line *line)
{
	if( line->words)
	{
		t_link *l;
		for( l = line->words->first; l; l = l->next)
		{
			t_word *word = ( t_word *) l->data; 
			word_free( word);
		}

		lst_cleanup( line->words);
	}

	if (line->data) free( line->data);

	free( line);
}

// FILE
int file_lines_size( t_file *file)
{
	int size = 0;
	t_link *l;
	t_line *line;
	for( l = file->lines->first; l; l = l->next)
	{
		line = ( t_line *) l->data;
		size += line->size;  // add newline space
	}

	return size;
}

int file_write_data( t_file *file)
{
	int size = file_lines_size( file);
	t_link *l;
	t_line *line;

	free( file->data);
	//file->data = ( char *) calloc( 1,sizeof(char) * size);
	file->data = ( char *) malloc( sizeof(char) * size);
	char *ptr = file->data;

	for( l = file->lines->first; l; l = l->next)
	{
		line = ( t_line *) l->data;
		if( line)
		{
			int s = line->size;
			memcpy( ptr, line->data, s);
			ptr += s-1;
			*ptr = (int)'\n';
			ptr++;
		}
	}

	return 1;
}

int file_write_lines( t_file *file)
{
	int size = file_lines_size( file);
	file_write_data( file);
	file_write( file, file->data, size);
	return 1;
}

/** store lines without \n **/
int file_read_lines(t_file *file)
{
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

		file->tot_line = totline;

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

t_line *file_line_get( t_file *file, int p)
{
	if( file->data)
	{
		if( file->lines)
		{
			t_line *line = ( t_line *) lst_get_by_range( file->lines, p);
			if( line) 
			{
				return line;
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


char *file_line_data_get( t_file *file, int p)
{
	t_line *line = file_line_get( file, p);
	return line->data;
}
