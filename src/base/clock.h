/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __CLOCK_H
#define __CLOCK_H

#include "base.h"

#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Clock
{
	t_id id;

	float freq;
	int factor;
	double delta;
	float limit; 
	struct timeval before; 
	struct timeval now;

	int framerate;
	struct timeval frame_before; 
	struct timeval frame_now;

	struct timeval start;
	unsigned long start_time; //milliseconds

	unsigned int msec;
	int sec;
	int min;

}t_clock;

double 		clock_now_sec_precise( void);
void 		clock_init( t_clock *clock);
void 		clock_free( t_clock *clock);
int 		clock_get_delta_sec( t_clock *clock);
void 		clock_time_set(t_clock *clock);
int 		clock_get_second( t_clock *clock);
int		clock_get_delta_usec( t_clock *clock);
int 		clock_convert_usec_to_millisec( int usec);

t_clock *	clock_new( const char *name);

#ifdef __cplusplus
}
#endif

#endif
