/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "context.h"
#include "node.h"
#include "scene.h"
#include "app.h"
#include "op.h"
#include "event.h"
#include "term.h"
#include "camera.h"
#include "object.h"
#include "list.h"
#include "vector.h"
#include "data.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "sketch.h"

#define TERM_TOT_LINE 20
#define TERM_LINE_LENGTH 30

void term_line_add( t_term *term, char *data)
{
	t_txt *line = txt_new(data);
	lst_add( term->lines, line, "line");
	term->height += 20;
}

void term_push(t_term *term, t_link *l, const char *name)
{
	t_txt *t;
	t=l->data;
	if( l->prev)
	{
		term_push( term, l->prev, t->name);
		t->data_change( t, name);
	}
	else
	{
		t->data_change( t, name);
	}
}

void term_print( t_term *term, char *data)
{
	// Add New Line
	if(term->line < term->tot_line -1)
	{
		term_line_add(term,data);
		term->line++;
	}
	// Or Push
	else 
	{
		term_push( term, term->lines->last, data);
	}
}

void term_log(char *fmt, ...)
{
	t_context *C = ctx_get();
	char msg[400];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(msg,fmt,ap);
	va_end(ap);

	term_print(C->term,msg);
}

void term_echo( t_term *term, char *fmt, ...)
{
	char msg[400];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(msg,fmt,ap);
	va_end(ap);

	term_print(term,msg);
}

// DRAW

void term_draw(t_term *term)
{
	t_link *l;
	t_txt *t;

	float col[] = { 1,1,1};
	float a[] = { 0, 0, 0};
	float c[] = { 0, -term->height, 0, 0};


	float width = term->width;
	glPushMatrix();

		glTranslatef(0,-20,0);
		for(l=term->lines->first;l;l=l->next)
		{
			t=l->data;
			t->draw(t);
			if(t->width > width) width = t->width;
			glTranslatef(0,-20,0);
		}

	glPopMatrix();

	float h[3] = { width + 15 , 0, 0};
	float h2[3] = { width + 15 , -term->height, 0};

	skt_line( a, h, 1, col);
	skt_line( c, h2, 1, col);

	term->width = width;
}

// RESET

void term_reset(t_term *term)
{
	t_link *link;
	t_txt *line;

	for(link = term->lines->first; link; link = link->next)
	{
		line = link->data;
		txt_free(line);
	}

	lst_cleanup(term->lines);
	term->cursor = NULL;
	term->is_init = 0;

	term->line=0;
}

// FREE

void term_free(t_term *term)
{
	t_link *link;
	t_txt *line;

	if(term->lines->first)
	{
		for(link = term->lines->first; link; link = link->next)
		{
			line = link->data;
			txt_free(line);
		}
	}

	lst_free(term->lines);
	link_free(term->cursor);

	mem_free( term, sizeof( t_term));
}

// NEW

t_term *term_new( const char *name)
{
	t_term *term  = (t_term *)mem_malloc(sizeof(t_term));

	id_init( &term->id, name);

	term->tot_line = TERM_TOT_LINE;
	term->line = 0;
	term->line_length = TERM_LINE_LENGTH;
	term->offset = 0;
	term->cursor = NULL;
	term->lines = lst_new("lst");
	term->is_init = 0;
	term->loc_x = 0;
	term->loc_y = 0;
	term->draw = term_draw;
	term->width = 250;
	term->height = 20;

	return term;
};

// INIT

void term_init(void)
{
	t_context *C = ctx_get();
	lst_add( C->terms, C->term, "term_main");

	t_term *term_event = term_new( "term_event");
	lst_add( C->terms, term_event, "term_event");

	t_term *term_state = term_new( "term_state");
	lst_add( C->terms, term_state, "term_state");
}

// GET

t_term *term_get( const char *name)
{
	t_context *C = ctx_get();
	t_link *l = lst_link_find_by_name( C->terms, name);
	if( l) return l->data;
	else return NULL;
}



