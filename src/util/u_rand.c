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

int u_randn(int density)
{
	int num = rand()%density;

	return num;
}

int u_randrange(int start,int end)
{
	//srand(time(NULL));
	int n;
	if (start != end)  n = rand()%(end-start) + start;
	else n =start;
	return n;
}

int u_randrange_relative(int start,int end)
{
	int n;

	//srand(time(NULL));
	if (start != end)  n = rand()%(end-start) -((end-start)/2);
	else n =start;
	return n;
}


