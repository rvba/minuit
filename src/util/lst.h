/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "common.h"

// LINK

typedef struct Link
{
	struct ID id;

	int order;		// Used by rhizome sorting

	struct Link *next;
	struct Link *prev;

	void *data;

}t_link;

// LST

typedef struct Lst
{
	struct ID id;

	t_link *first;
	t_link *last;
	t_link *current;

	t_data_type type;

	int count;

}t_lst;
