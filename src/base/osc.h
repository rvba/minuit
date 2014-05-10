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

#ifdef __cplusplus
extern "C" {
#endif

int osc_send( const char *port, const char *msg, const char *format, ...);
int osc_server( int port);
int osc_client( int port);

#ifdef __cplusplus
}
#endif


#endif
