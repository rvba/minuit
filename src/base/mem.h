/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __MEM_H_
#define __MEM_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern size_t MEM_SIZE;

void *mem_malloc(size_t size);
void mem_free( void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif
