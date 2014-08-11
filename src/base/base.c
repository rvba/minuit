/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

/*
size_t get_memory_peak(void)
{
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
    return (size_t)(rusage.ru_maxrss * 1024L);
}
*/

void rest( float sec)
{
	usleep( (int)( sec * MILLION));
}

size_t get_memory_usage(void)
{
    long rss = 0L;
    FILE* fp = NULL;
    if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
        return (size_t)0L;     	// open error 
    if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    {
        fclose( fp );
        return (size_t)0L;     // read error
    }
    fclose( fp );
    return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);
}

void set_name(char *dst,const char *src)
{
	strncpy(dst,src,_NAME_);
	dst[_NAME_-1]='\0';
}

void set_name_long(char *dst,const char *src)
{
	strncpy(dst,src,_NAME_LONG_);
	dst[_NAME_LONG_-1]='\0';
}

void set_path(char *dst,const char *src)
{
	strncpy(dst,src,_PATH_);
	dst[_PATH_-1]='\0';
}

void id_show( t_id *id)
{
	printf("[id] id:%d name:%s store:%d\n", id->id, id->name, id->store);
}

void id_init(t_id *id, const char *name)
{
	id->id = 0;
	id->id_chunk = 0;
	id->users = 0;
	set_name(id->name, name);
	id->store = 0;
	id->node = NULL;
}

void color_id(int *color)	
{
	color[0]++;

	if(color[0] > 255)
	{
		color[0] = 0;
		color[1]++;
		if(color[1] > 255)
		{
		     color[1] = 0;
		     color[2]++;
		}
	}
}


