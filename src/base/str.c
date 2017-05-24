/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

void s_increment( char *str, int max_length)
{
	if(str)
	{
		int i;
		int last_char = 0;
		int digit_length = 0;

		/* get last letter */
		for( i = 0; i < max_length; i++)
		{
			if( str[i] == '\0')
			{
				last_char = i;
				break;
			}
		}

		/* get digit length */
		for( i = last_char-1; i > 0 ; i--)
		{
			if( !isdigit(str[i]))
			{
				break;
			}

			digit_length++;
		}

		/* increment */
		if( digit_length > 0)
		{
			/* find number */
			int word_length = last_char;
			int digit_position = word_length - digit_length;
			int n = atoi(str+digit_position);
			snprintf(str+digit_position,max_length-digit_position,"%d", ++n);
		}
		else
		/* new digit */
		{
			str[last_char] = '0';
		}
	}
}

void s_cp( char *dst, const char *src, int size)
{
	strncpy( dst, src, size);
}

void s_cat( char *dst, const char *src, int size)
{
	strncat( dst, src, size);
}

void s_show( const char *str)
{
	printf("%s\n", str);
}

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
	char *string=(char *)mem_malloc(sizeof(char)*(strlen(data)+1));
	strcpy(string,data);
	return string;
}

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

void s_remove_newline( char *target, char *src, int size)
{
	int s = 0;
	bzero( target, size);
	while( *src != '\n' && s <= size)
	{
		*target = *src;
		target++;
		src++;
		s++;
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

void s_convert_endline_newline( char *target, char *src)
{
	while( *src!='\0')
	{
		*target = *src;
		target++;
		src++;
	}

	*target ='\n';
}

void s_store(char *target,char source,int pos)
{
	target[pos] = source;
}


