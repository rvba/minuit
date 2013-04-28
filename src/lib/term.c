/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

#define CTX_TERM_TOT_LINE 50
#define CTX_TERM_LINE_LENGTH 30


void term_line_add(t_term *term,char *data)
{
	t_txt *line=txt_new(data);
	lst_add(term->lines,line,"line");
}

void term_line_cpy(t_txt *target,t_txt *source)
{
	char *name=source->name;
	target->data_change(target,name);
}

void term_offset(t_term *term)
{
	t_link *l;
	t_txt *next_line;

	for(l=term->lines->first;l;l=l->next)
	{
		t_link *next=l->next;

		if(next)
		{
			next_line=next->data;
			term_line_cpy(l->data,next_line);
		}
	}
}

void term_loop(t_term *term)
{
	t_link *l;
	t_txt *t;
	for(l=term->lines->first;l;l=l->next)
	{
		t=l->data;
		if(l->next)
		{
			t_txt *target=l->next->data;
			char *name=target->name;
			t->data_change(t,name);
		}
	}
}

void term_print(t_term *term,char *data)
{
	if(term->line < term->tot_line -1)
	{
		term_line_add(term,data);
		term->line++;
	}
	else 
	{
		if(!term->is_init)
		{
			term->cursor=term->lines->last;
			term->is_init=1;
		}

		term_loop(term);
		t_txt *target=term->cursor->data;
		target->data_change(target,data);
	}
}

void term_log(char *fmt, ...)
{
	t_context *C = ctx_get();
	char msg[40];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(msg,fmt,ap);
	va_end(ap);

	term_print(C->term,msg);
}

void term_echo(t_term *term,char *fmt, ...)
{
	char msg[40];
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

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(term->loc_x,term->loc_y,0);

	for(l=term->lines->first;l;l=l->next)
	{
		t=l->data;
		t->draw(t);
		glTranslatef(0,-20,0);
	}
	glPopMatrix();
}

// INIT

void term_init(t_term *term)
{
	t_context *C=ctx_get();

	term->loc_x=20;
	term->loc_y=C->app->window->height*.9;
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

	free(term);
}

// NEW

t_term *term_new(const char *name)
{
	t_term *term  = (t_term *)malloc(sizeof(t_term));

	set_name(term->name,name);

	term->tot_line=CTX_TERM_TOT_LINE;
	term->line=0;
	term->line_length=CTX_TERM_LINE_LENGTH;
	term->offset=0;
	term->cursor=NULL;
	term->lines=lst_new("lst");
	term->is_init=0;
	term->loc_x=0;
	term->loc_y=0;

	term->draw=term_draw;
	term->init=term_init;

	return term;
};

