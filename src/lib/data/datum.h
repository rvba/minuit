/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __DATUM_H
#define __DATUM_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;


typedef struct Datum
{
	t_id id;
	t_data_type type;
	int count;
	size_t size;
	void *data;
}t_datum;

void datum_rebind( struct Scene *sc,void *ptr);
void *datum_new( const char *name);
t_datum *datum_add( t_data_type type, const char *name, void *data);
void datum_free(t_datum *datum);
void datum_init( t_datum *datum);



#ifdef __cplusplus
}
#endif



#endif
