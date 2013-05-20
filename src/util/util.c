/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"


/*
size_t get_memory_peak(void)
{
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
    return (size_t)(rusage.ru_maxrss * 1024L);
}
*/

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

inline void negate_int(void *_dst)
{
	int *dst = _dst;
	*dst = - (*dst);
}

inline void negate_float(void *_dst)
{
	float *dst = _dst;
	*dst = - (*dst);
}

inline void set_int(void *_ptr,int i)
{
	int *ptr = (int *)_ptr;
	*ptr=i;
}

inline void set_uint(void *_ptr,int i)
{
	unsigned int *ptr = (unsigned int *)_ptr;
	*ptr=(unsigned int) i;
}

inline void set_float(void *_ptr,float i)
{
	float *ptr = (float *)_ptr;
	*ptr=i;
}

inline void flow_int_int(void *_dst, void *_src)
{
	int *dst = _dst;
	int *src = _src;

	*dst = *src;
}

inline void flow_int_float(void *_dst, void *_src)
{
	int *dst = _dst;
	float *src = _src;

	*dst = (int) *src;
}

inline void flow_float_float(void *_dst, void *_src)
{
	float *dst = _dst;
	float *src = _src;

	*dst = *src;
}

inline void flow_float_int(void *_dst, void *_src)
{
	float *dst = _dst;
	int *src = _src;

	*dst = (float)*src;
}

inline void srf_float(void *ptr, void *data, int indice)
{
	float *pointer = (float *) ptr;
	float *d = (float *) data;
	*(pointer + indice)  = *d;
}

inline void srf_uint(void *ptr, void *data, int indice)
{
	unsigned int *pointer = (unsigned int *) ptr;
	unsigned int *d = (unsigned int *) data;
	*(pointer + indice) = *d;
}

inline void *grf_float(void *ptr, int indice)
{
	float *pointer = (float *)ptr;
	return pointer + indice;
}

inline void *grf_uint(void *ptr, int indice)
{
	unsigned int *pointer = (unsigned int  *)ptr;
	return pointer + indice;
}

inline int drf_int(void *ptr)
{
	return *((int *)ptr);
}

inline int drf_uint(void *ptr)
{
	return *((unsigned int *)ptr);
}

inline float drf_float(void *ptr)
{
	return *((float *)ptr);
}

inline char drf_char(void *ptr)
{
	return *((char *)ptr);
}

inline char *drf_string(void *ptr)
{	
	return (char *)ptr;
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

inline int is(const char a[],const char b[])
{
	if(strcmp(a,b)==0) return 1;
	else return 0;
}

void clock_init(t_clock *clock)
{
	struct timeval tv;
	gettimeofday(&clock->start,NULL);
	gettimeofday(&tv,NULL);
	clock->start_time=tv.tv_sec * 1000 + tv.tv_usec/1000;
	clock->limit=0.05;
}

void clock_free(t_clock *clock)
{
	free(clock);
}

// CLOCK

t_clock *clock_new(void)
{
	t_clock *clock = (t_clock *)malloc(sizeof(t_clock));

	clock->freq=1.0;
	clock->delta=0;
	clock->factor=0;
	clock->limit = PROCESS_LIMIT;
	clock->now.tv_sec=0;
	clock->now.tv_usec=0;
	clock->before.tv_sec=0;
	clock->before.tv_usec=0;

	clock->framerate=25;
	clock->frame_before.tv_sec=0; 
	clock->frame_before.tv_usec=0; 
	clock->frame_now.tv_sec=0;
	clock->frame_now.tv_usec=0;
	
	clock->start.tv_sec=0;
	clock->start.tv_usec=0;

	clock->msec=0;
	clock->sec=0;
	clock->min=0;

	return clock;
}








