/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "common.h"
#include "util.h"
#include <time.h>

void rnd_init(void)
{
	srand(time(NULL));
}

int rnd_range(int start,int end)
{
	// end >= start
	return (rand() % (end - start + 1) + start);
}

int rnd(int n)
{
	return (rand() % (n + 1) );
}

float rndf(void)
{
	return ( (float) rnd(1000) * 0.001 );
}

float rndr(void)
{
	return ( ( rndf() - 0.5) * 2.0f );
}

