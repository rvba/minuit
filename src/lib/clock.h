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

#include <ctype.h>
#include <time.h>
#include <sys/time.h>

typedef struct Clock
{
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


void clock_init(t_clock *clock);
void clock_free(t_clock *clock);
t_clock *clock_new(void);

#endif
