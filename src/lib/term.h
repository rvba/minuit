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

#include "txt.h"
#include "util.h"

#define TERM_LIMIT 30

typedef struct Term t_term;

struct Term
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	int tot_line;
	int line;
	int line_length;
	int offset;

	t_lst *lines;
	t_link *cursor;

	int is_init;
	float loc_x;
	float loc_y;

	void (* draw)(t_term *term);
	void (* init)(t_term *term);
};

void term_reset(t_term *term);
void term_free(t_term *term);
t_term *term_new(const char *name);
void term_print(t_term *term,char *data);
void term_log(char *fmt, ...);
void term_echo(t_term *term,char *fmt, ...);

#endif
