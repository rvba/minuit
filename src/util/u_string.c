/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"

void s_raw_cp(char *src,char *dst) 
{
	while(*src!='\0')
	{
		*dst=*src;
		src++;
		dst++;
	}
}

char *s_allocate(const char *data)
{
	char *string=(char *)malloc(sizeof(char)*(strlen(data)+1));
	strcpy(string,data);
	return string;
}

/** format time in yy/mm/hh/ss with a char[30] **/
void s_write_time(char *string)
{
	time_t tim=time(NULL);
	struct tm *now=localtime(&tim);
	snprintf(string,30,"./%d%02d%02d%02d%02d%02d.png",now->tm_year+1900, now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
}

// work with cursor

int s_append(char *dest,char *src,int pos)
{
	if(src)
	{
		while(*src!='\0')
		{
			dest[pos]=*src;
			pos++;
			src++;
		}
	}
	// set end
	else
	{
		dest[pos]='\0';
		pos++;
	}
	return pos;
}

void s_remove_newline(char *target,char *src)
{
	while(*src!='\n')
	{
		*target=*src;
		target++;
		src++;
	}
}

void s_truncate_after(char letter,char *target,char *src)
{
	while(*src!=letter)
	{
		src++;
	}

	src++;

	while(*src!='\0')
	{
		*target=*src;
		src++;
		target++;
	}

	*target='\0';
}

void s_convert_newline_endline(char *target,char *src)
{
	while(*src!='\n')
	{
		*target=*src;
		target++;
		src++;
	}

	*target='\0';
}

void s_store(char *target,char source,int pos)
{
	target[pos] = source;
}

