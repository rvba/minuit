/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#define PATH_LIMIT 1024
#define S_DEBUG 0

int file_path_split(t_file *file)
{
	int i,j;
	char *letter;
	int npart=0;
	int *size;
	int indice=-1;
	int start=0;

	char *path = file->path;

	// RELATIVE PATH: if path is relative, go to first slash ./
	if (path[0] == '.')
	{
		path++;
	}

	// SLASHES: count slashes
	for(letter=path;*letter!='\0';letter++)
	{
		if(*letter == '/')
			npart++;
	}

	if(S_DEBUG) printf("%d part(s) in %s\n",npart,path);

	if(npart==0)
	{
		file->path_tot_parts=0;
		return 0;
	}
	// BUILD: path_parts
	else
	{
		// size: length of parts
		size = (int *)malloc(sizeof(int)*npart);

		for(letter=path;*letter!='\0';letter++)
		{
			if(*letter == '/')
			{
				indice++;
				size[indice]=0;
			}
			else
			{
				size[indice]++;
			}
		}

		// path_parts
		file->path_parts = (char**)malloc(sizeof(char *) * npart);

		// allocate parts
		start=0;
		for(i=0;i<npart;i++)
		{
			//first slash
			start++;

			// add 2 more char : start from 0 +  one for endline
			//file->path_parts[i] = (char*)malloc(sizeof(char)*(size[i]+2));
			//XXX
			file->path_parts[i] = (char*)malloc(sizeof(char)*(size[i]+1));

			for(j=0;j<size[i];j++)
			{
				file->path_parts[i][j] = path[start];
				start++;
			}

			file->path_parts[i][j] = '\0';

			if(S_DEBUG) printf("element:%s\n",file->path_parts[i]);
		}

		file->path_tot_parts=npart;

		// free
		free(size);

		return 1;
	}
}
	
int file_init(t_file *file)
{
	int i,j;
	char *letter;
	char *filename;
	int indice=-1;
	int is_split;

	if(strlen(file->path)==0)
	{
		printf("null path\n");
		return 0;
	}

	is_split=file_path_split(file);

	if(is_split)
	{
		filename=file->path_parts[file->path_tot_parts-1];
	}
	else
	{
		filename=file->path;
	}

	// split name/extension

	indice = 0;
	int with_extension=0;

	// check last element
	for(letter=filename;*letter!='\0';letter++)
	{
		if(*letter == '.')
		{
			with_extension=1;
			break;
		}
		else
		{
			indice++;
		}
	}

	if(with_extension)
	{
		i=0;

		for(letter=filename;*letter!='.';letter++)
		{
			file->name[i] = *letter;
			i++;
		}

		file->name[i] = '\0';

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
		set_name(file->name,file->path);
	}


	return 1;
}

void file_build_path(t_file *file)
{
	int i;
	int cursor=0;

	char new_path[PATH_LIMIT];

	// concatane
	for(i=0;i<file->path_tot_parts;i++)
	{
		char *add=file->path_parts[i];
		new_path[cursor]='/';
		cursor++;
		while(*add!='\0')
		{
			new_path[cursor]=*add;
			add++;
			cursor++;
		}
	}

	new_path[cursor]='\0';

	set_path(file->path,new_path);
}

void file_go_backward(t_file *file)
{
	if(file->path_tot_parts>1)
	{
		file->path_tot_parts--;
		free(file->path_parts[file->path_tot_parts]);
		file_build_path(file); 
	}
	else
	{
		printf("no backward possible\n");
	}
}

void file_element_add(t_file *file,char *name)
{
	file->path_parts=(char **)realloc(file->path_parts,sizeof(char *)*file->path_tot_parts+1);
	file->path_parts[file->path_tot_parts]=s_allocate(name);
	file->path_tot_parts++;
}

void file_go_directory(t_file *file,char *name)
{
	file_element_add(file,name);
	file_build_path(file); 
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
	if(file->path) printf("path:%s\n",file->path);
	if(file->name) printf("name:%s\n",file->name);
	if(file->ext) printf("ext:%s\n",file->ext);
	printf("path_tot_parts:%d\n",file->path_tot_parts);
	if(file->path_tot_parts)
	{
		int i;
		for(i=0;i<file->path_tot_parts;i++)
		{
			printf("elem %d:%s\n",i,file->path_parts[i]);
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
		file->file=fopen(file->path,"w");
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
	FILE *f = fopen(file->path,"r");
	fseek (f,0,SEEK_END);
	file->data_size = ftell(f);
	rewind(f);
	file->data = (char *)malloc(sizeof(char)*file->data_size);
	fread (file->data,1,file->data_size,f);
}

void file_free(t_file *file)
{
	if(file->path_parts)
	{
		int i;
		for(i=0;i<file->path_tot_parts;i++)
		{
			free(file->path_parts[i]);
		}
		free(file->path_parts);
	}
}

t_file *file_new(const char *path)
{
	t_file *file = (t_file *)malloc(sizeof(t_file));

	memset(file->name,'\0',_NAME_);
	memset(file->ext,'\0',_EXT_);
	memset(file->path,'\0',_PATH_);

	set_path(file->path,path);

	file->path_parts=NULL;
	file->data=NULL;
	file->lines=lst_new("lst");
	file->file=NULL;

	file->path_tot_parts=0;
	file->data_size=0;
	file->tot_line=0;

	// SPLIT
	file_init(file);

	return file;
}



