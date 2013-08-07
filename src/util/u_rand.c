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


/*
int u_randn(int density)
{
	int num = rand()%density;

	return num;
}

int u_randrange(int start,int end)
{
	int n;
	if (start != end)  n = rand()%(end-start) + start;
	else n =start;
	return n;
}

int u_randrange_relative(int start,int end)
{
	int n;

	if (start != end)  n = rand()%(end-start) -((end-start)/2);
	else n =start;
	return n;
}
*/

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

