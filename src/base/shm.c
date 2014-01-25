/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "util.h"

key_t shm_key( const char *file)
{
	key_t key;
	if ((key = ftok( file, 'R')) == -1)
	{
		printf("SHM ERROR ftok\n");  
		return 0;
	}
	else
	{ 
		//printf("SHM key OK\n");
		return key;
	}
}

int shm_get( key_t key, int size)
{
	int shmid;
	if ((shmid = shmget(key, size, 0644 | IPC_CREAT)) == -1)
	{
		printf("SHM ERROR get\n");
		return 0;
	}
	else
	{
		//printf("SHM GET OK > %d\n", shmid);
		return shmid;
	}
}

void *shm_attach( int id)
{
	void *ptr;

	ptr = shmat( id, (void *)0, 0);

	if ( ptr == (char *)(-1))
	{
		printf("SHM ERROR attach\n");
		return NULL;
	}
	else
	{
		printf("SHM ATTACH OK %p\n", ptr);
		return ptr;
	}
}

int shm_detach( void *ptr)
{
	if ( shmdt( ptr) == -1)
	{
		printf("SHM ERROR detach\n");
		return 0;
	}
	else
	{
		//printf("SHM DETACH OK\n");
		return 1;
	}
}


