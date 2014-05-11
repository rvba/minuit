/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "clock.h"
#include "util.h"

/*
struct timeval {
       time_t      tv_sec;     // seconds 
       suseconds_t tv_usec;    // microseconds ( 1 / 1 000 000 )
   };
*/

int clock_to_sec( unsigned int time)
{
	return time / 1000;
}

double clock_now_sec_precise( void)
{
	struct timeval time;
	gettimeofday( &time, NULL);
	return(  ( double) time.tv_sec  + ((double)time.tv_usec / 1000000));
}

unsigned int clock_now_usec( void)
{
	struct timeval time;
	gettimeofday( &time, NULL);
	return(  (time.tv_sec * 1000000) + time.tv_usec );
}

int clock_now_sec( void)
{
	struct timeval time;
	gettimeofday( &time, NULL);
	return(  time.tv_sec );
}

int clock_get_second( t_clock *clock)
{
	return clock->start.tv_sec;
}

int clock_get_usec( t_clock *clock)
{
	return ( (clock->start.tv_sec * 1000000) + ( clock->start.tv_usec));
}

int clock_get_delta_sec( t_clock *clock)
{
	int past = clock_get_second( clock);
	int now =  clock_now_sec();
	int delta = now - past;

	return delta;
}

int clock_get_delta_usec( t_clock *clock)
{
	int past = clock_get_usec( clock);
	int now =  clock_now_usec();
	int delta = now - past;

	return delta;
}

void clock_init(t_clock *clock)
{
	struct timeval tv;
	gettimeofday(&clock->start,NULL);
	gettimeofday(&tv,NULL);
	clock->start_time=tv.tv_sec * 1000 + tv.tv_usec/1000;
	clock->limit=0.05;
}

void clock_time_set(t_clock *clock)
{
	struct timeval tv;
	gettimeofday(&clock->start,NULL);
	gettimeofday(&tv,NULL);
	clock->start_time = tv.tv_sec * 1000 + tv.tv_usec/1000;
}

void clock_free(t_clock *clock)
{
	mem_free( clock, sizeof( t_clock));
}

// CLOCK

t_clock *clock_new( const char *name)
{
	t_clock *clock = (t_clock *)mem_malloc(sizeof(t_clock));

	id_init( &clock->id, name);

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


