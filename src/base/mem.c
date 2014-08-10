/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include <stdlib.h>

size_t MEM_SIZE = 0;

void *mem_malloc(size_t size)
{
	MEM_SIZE += size;
	return( malloc( size));
}

void mem_free( void *ptr, size_t size)
{
	MEM_SIZE -= size;
	free( ptr);
}
