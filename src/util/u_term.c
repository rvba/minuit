/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"

char u_read_char(void)
{
	char c;
	scanf("%c",&c);
	return c;
}
int u_read_int(void)
{
	int n;
	scanf("%d",&n);
	return n;
}

void print_args(int argc,char **argv)
{
	int i;
	for(i=0;i<argc;i++)
	{
		printf("%s\n",argv[i]);
	}
}
