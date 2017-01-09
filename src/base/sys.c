/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

void rest( float sec)
{
	usleep( (int)( sec * MILLION));
}

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

int sys_file_exists( const char *path)
{
	if( access( path, F_OK ) != -1 ) return 1;
	else return 0;
}

void sys_get_cd(const char *cmd)
{
	FILE *fp;
	char input[1024];

	fp = popen(cmd, "r");

	if(fp)
	{
		while (fgets(input, sizeof(input)-1, fp) != NULL)
		{
			printf("%s",input);
		}
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

int sys_get( const char *cmd, char *data, int size)
{
	FILE *fp;
	char input[ size];

	fp = popen( cmd, "r");

	if(fp)
	{
		while ( fgets ( input, sizeof( input) - 1, fp) != NULL)
		{
		}

		s_remove_newline( data, input, size);
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

