/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __TERM_H
#define __TERM_H

#include <stdarg.h>

#include "txt.h"
#include "util.h"

#define TERM_LIMIT 30
#define TERM_SIMPLE 1
#define TERM_DYNAMIC 2

struct Lst;
struct Link;

typedef struct Term
{
	t_id id;

	int tot_line;
	int line;
	int line_length;
	int offset;

	float width;
	float height;

	struct Lst *lines;
	struct Link *cursor;

	int is_init;
	float loc_x;
	float loc_y;

	void (* draw)(struct Term *term);
	void (* init)(struct Term *term);

	struct Block *block;
	int type;
	int visible;

}t_term;

void 		term_reset_all( void);
void 		term_reset (t_term *term);
void 		term_free( t_term *term);
void *		term_new( const char *name);
void 		term_init( void);
void 		term_print( t_term *term, const char *msg);
void 		term_log( const char *fmt, ...);
void 		term_l( t_term *term, const char *fmt, ...);
void 		term_echo( t_term *term, char *fmt, ...);
t_term *	term_get( const char *name);

#endif
