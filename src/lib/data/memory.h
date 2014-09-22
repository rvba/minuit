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

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;

typedef struct Chunk t_chunk;

typedef enum Chunk_Type
{	
	ct_node,
	ct_data
}t_chunk_type;

struct Chunk
{
	t_chunk_type chunk_type;
	t_data_type type;
	int id;			// chunk id (order in Static MEMORY)

	int size;		// block size
	int tot;		// amount of blocks

	void *pointer;		// pointer to data 
};

int		mem_add( t_chunk_type chunk_type, t_data_type type, size_t size, int tot, void *pointer);
void 		mem_show(void);
void 		mem_init(void);
void 		mem_write(const char *path);
void *		chunk_new(t_chunk_type chunk_type,t_data_type type,size_t size,int tot,void *pointer);
const char *	chunk_type_get(t_chunk_type type);

void mem_read( struct Scene *sc, const char *path);

#ifdef __cplusplus
}
#endif

#endif
