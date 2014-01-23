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


void sys_get_cd(const char *cmd)
{
	FILE *fp;
	char input[1024];

	fp = popen(cmd, "r");

	if(fp)
	{
		printf("BROWSER\n");
		while (fgets(input, sizeof(input)-1, fp) != NULL)
		{
			printf(">%s",input);
		}
		printf("DONE\n");
	}
}

int sys_get_hostname( char* name, int size)
{
	FILE *fp;
	char input[ size];

	fp = popen("hostname", "r");

	if(fp)
	{
		while ( fgets ( input, sizeof( input) - 1, fp) != NULL)
		{
		}

		s_remove_newline( name, input, size);
		pclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}

void get_log(char *dst,char *msg)
{
	int i=0;
	int start=0;
	char *m=msg;

	while(*m)
	{
		// start at first digit
		if(isdigit(*m))
		{
			start=1;
		}
		if(start)
		{
			// ommit new line
			if(!isspace(*m))
			{
				dst[i]=*m;
				i++;
			}
		}
		m++;
	}

	// add end line
	dst[i+1]='\0';
}

