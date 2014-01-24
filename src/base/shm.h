/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SHM_H
#define __SHM_H

#include <sys/ipc.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

key_t	shm_key( const char *file);
int 	shm_get( key_t key, int size);
void *	shm_attach( int id);
int 	shm_detach( void *ptr);


#ifdef __cplusplus
}
#endif


#endif
