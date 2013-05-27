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


