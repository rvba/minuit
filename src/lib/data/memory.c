/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "memory.h"
#include "log.h"
#include "list.h"
#include "base.h"

#define DEBUG 1

t_lst *MEMORY;
int indice;
t_node *node=NULL;
t_node *tmp_node;

/*
	********************************
	SHOW
	********************************
*/

const char *chunk_type_get( t_chunk_type type)
{
	switch(type)
	{
		case(ct_node):return "chunk_node";break;
		case(ct_data):return "chunk_data";break;
		default:	printf("[ERROR chunk_get_type] Unknwon type:%d\n",type);
				return "chunk_null";
				break;
	}
}


void chunk_show(t_chunk *c)
{
	printf("chunk chunk_type:%s type:%s id:%d size:%d tot:%d ptr:%p\n",
		chunk_type_get(c->chunk_type),data_name_get(c->type),c->id,c->size,c->tot,c->pointer);
}

void mem_show(void)
{
	t_link *l;
	t_chunk *c;
	for(l=MEMORY->first;l;l=l->next)
	{
		c=l->data;
		chunk_show(c);
	}
}

/*
	********************************
	NEW
	********************************
*/

void *chunk_new(t_chunk_type chunk_type,t_data_type type,size_t size,int tot,void *pointer)
{
	t_chunk *chunk=(t_chunk *)mem_malloc(sizeof(t_chunk));

	chunk->chunk_type=chunk_type;
	chunk->type=type;

	// get id
	chunk->id=indice;			
	indice++;

	chunk->size=size;			// size of element
	chunk->tot=tot;				// amount of elements
	chunk->pointer=pointer;			// data to be saved

	return chunk;
}

/*
	********************************
	READ
	********************************
*/

// ID STORE

void id_store( t_scene *sc, t_chunk *c, void *data)
{
	t_lst *lst;

	// store node
	if( c->chunk_type == ct_node)
	{
		lst = sc->tmp_node;

		// var node: store tmp node
		if( c->type == dt_var)
		{
			tmp_node = ( t_node *) data;
		}
	}
	// store struct
	else
	{
		lst = sc->tmp_data;

		// var data: bind tmp node
		if( c->type == dt_var)
		{
			if( tmp_node)
			{
				tmp_node->data = data;
				tmp_node = NULL;
			}
		}
	}

	// add to LST
	lst_add( lst, data, "data");
}

void mem_read( t_scene *sc, const char *path)
{
	void *data; 		// struct memory allocation
	char magic;		// read magic
	int limit=0;		// max size
	#ifdef WITH_DEBUG
	int file_size;		// size of bin
	#endif

	t_chunk *c;

	// FILE
	FILE *file=fopen(path,"r");

	// FILE SIZE
	fseek (file,0,SEEK_END);
	#ifdef WITH_DEBUG
	file_size = (int)ftell(file);
	#endif
	rewind(file);

	// NEW CHUNK
	c=chunk_new(ct_node,dt_null,0,0,NULL);

	ulog((LOG_READ,"File size:%d\n",file_size));
	ulog((LOG_READ,"[0]\n",file_size));

	size_t rsize;

	// READ FILE
	while(1)
	{
		// LIMIT
		if(limit>BIN_MAX)
		{
			printf("[ERROR ctx_load] Max Bin\n]");
			break;
		}

		// MAGIC
		rsize = fread(&magic,sizeof(char),1,file);
		if(rsize != 1) printf("read error 1\n");

		ulog((LOG_READ,"[%d][%d]\t(+%d)\tmagic {%c}\n",limit,(int)ftell(file),(int)sizeof(char),magic));

		// BREAK EOF
		if(magic=='&') break;

		// READ CHUNK
		rsize = fread(c,sizeof(t_chunk),1,file);
		(void) rsize;
		//if( rsize != (sizeof(t_chunk) * 1)) printf("read error 3\n");
		ulog((LOG_READ,"[%d]\t(+%d)\tchunk",(int)ftell(file),(int)sizeof(t_chunk)));
		ulog((LOG_READ," %s:%s:%d:%d\n",chunk_type_get(c->chunk_type),data_name_get(c->type),c->size,c->tot));

		// READ DATA

		// MALLOC
		data=mem_malloc(c->size*c->tot);

		// READ
		rsize = fread(data,c->size,c->tot,file);
		(void) rsize;
		//if(rsize != c->size * c->tot) printf("read error 4\n");

		ulog((LOG_READ,"[%d]\t(+%d)\tstruct %s:%s\n",(int)ftell(file),c->size,chunk_type_get(c->chunk_type),c->type));

		// STORE  Nodes + Structs
		id_store(sc,c,data);

		// LIMIT++
		limit++;
	}

	// CLOSE FILE

	fclose(file);
}


/*
	********************************
	WRITE
	********************************
*/

void mem_write( const char *path)
{
	// Open File
	FILE *file = fopen(path,"w");

	t_link *l;
	t_chunk *c;

	int check_size=0;

	ulog((LOG_SAVE,"[0]\n"));

	for(l=MEMORY->first;l;l=l->next)
	{
		c=l->data;
		// write magic
		fwrite("-",sizeof(char),1,file);

		check_size+=1;
		ulog((LOG_SAVE,"[%d]\t(+%d)\tmagic\n",(int)ftell(file),(int)sizeof(char)));

		// write chunk
		fwrite(c,sizeof(t_chunk),1,file);
		check_size+=(int)sizeof(t_chunk);

		ulog((LOG_SAVE,"[%d]\t(+%d)\tchunk\t",(int)ftell(file),(int)sizeof(t_chunk)));
		ulog((LOG_SAVE,"%s:%s:%d:%d:%p \n",chunk_type_get(c->chunk_type),data_name_get(c->type),c->size,c->tot,c->pointer));

		if(DEBUG)
		{
			if(c->chunk_type==ct_node)
			{
				node=c->pointer;
				if(node->type!=c->type)
				{
					printf("[ERROR mem_write] Wrong node %d cls type %s chunk type:%s chunk_id:%d\n",
						node->id,data_name_get(node->type),data_name_get(c->type),c->id);
				}
			}
			else
			{
				if(node)
				{
					if(node->type!=dt_var)
					{
						/*
						t_generic *g=(t_generic *)node->data;
						if(!is(g->name,node->cls->type))
						{
							printf("[ERROR mem_write] Wrong data type %s:%s\n",node->cls->type,g->name);
						}
						*/
					}

					node=NULL;

				}
				else
				{
					t_id *id = (t_id *) c->pointer;
					printf("[ERROR mem_write] No Node for data:%s,name:%s\n",data_name_get(c->type), id->name);
				}
			}
		}

		// write data
		fwrite(c->pointer,c->size,c->tot,file);
		check_size+=c->size*c->tot;

		ulog((LOG_SAVE,"[%d]\t(+%d)\t%s\n",(int)ftell(file),c->size,data_name_get(c->type)));

		// check for memory offset
		if(check_size!=ftell(file)) printf("[ERROR mem_write] Memory offset\n");
	}

	// write last character
	fwrite("&",sizeof(char),1,file);

	ulog((LOG_SAVE,"[%d]\t(+%d)\tmagic &\n",(int)ftell(file),(int)sizeof(char)));

	// close file
	fclose(file);
}

/*
	********************************
	ADD
	********************************
*/

// add new chunk to MEMORY return chunk_id
int mem_add( t_chunk_type chunk_type, t_data_type type, size_t size, int tot, void *pointer)
{
	if(!pointer) printf("[ERROR mem_store] NULL ptr\n");

	t_chunk *chunk = chunk_new( chunk_type, type, size, tot, pointer);
	lst_add( MEMORY, chunk, "chunk");

	ulog((LOG_MEMORY,"mem_store %d %s %s\n",chunk->id,chunk_type_get(chunk_type),data_name_get(type)));

	return chunk->id;
}

/*
	********************************
	INIT
	********************************
*/

// start indice at 1 (0 reserved for NULL id memory)
void mem_init(void)
{
	MEMORY = lst_new("lst");
	indice = 1;
}


