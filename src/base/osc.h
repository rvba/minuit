/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OSC_H__
#define __OSC_H__

#include "base.h"
#include "lo.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSC
{
	t_id id;
	int port;
	int done;
	lo_server_thread thread;

}t_osc;

typedef int (* osc_method)(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);

void osc_log_set( int i);
void osc_error(int num, const char *msg, const char *path);
int osc_send( const char *port, const char *msg, const char *format, ...);
int osc_send_ints( const char *port, const char *msg, int count, int *val);
int osc_server( int port);
int osc_client( int port);

t_osc *osc_new( const char *name);
t_osc *osc_server_new( int port);
void osc_server_start( t_osc *osc);
void osc_method_add( t_osc *osc, const char *msg, const char *type, osc_method f);

#ifdef __cplusplus
}
#endif


#endif
