/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "scene.h"

typedef struct Chunk t_chunk;
typedef enum Chunk_Type t_chunk_type;

enum Chunk_Type
{	
	ct_node,
	ct_data
};

struct Chunk
{
	t_chunk_type chunk_type;
	t_node_type type;
	int id;			// chunk id (order in Static MEMORY)

	int size;		// block size
	int tot;		// amount of blocks

	void *pointer;		// pointer to data 
};

int		mem_store(t_chunk_type chunk_type,t_node_type type,size_t size,int tot,void *pointer);
void 		mem_show(void);
void 		mem_read(void);
void 		mem_init(void);
void 		mem_write(const char *path);
t_chunk *	chunk_new(t_chunk_type chunk_type,t_node_type type,size_t size,int tot,void *pointer);
void 		mem_remove(int id);

void mem_free(void);

char *chunk_type_get(t_chunk_type type);

extern char chunk_name_null[];
extern char chunk_name_node[];
extern char chunk_name_data[];

#endif
