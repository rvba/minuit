/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __STR_H
#define __STR_H

void 	s_raw_cp( char *src, char *dst);
char *	s_allocate( const char *data);
void 	s_truncate_after( char letter, char *target, char *src);
void 	s_convert_newline_endline( char *target, char *src);
void 	s_remove_newline( char *target, char *src, int size);
void 	s_write_time( char *string);
int 	s_append( char *dest, char *src, int pos);
void 	s_store( char *target, char source, int pos);


#endif
