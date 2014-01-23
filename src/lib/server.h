/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SERVER_H_
#define __SERVER_H_

struct Socket;
struct Process;

typedef struct MNserver
{
	t_id id;
	int init;
	struct Process *process;
	struct MN_Socket *socket;

}t_server;

void 		server_start( t_server *server, int port);
void 		server_stop( t_server *server);
t_server *	server_new( const char *name);
void 		server_free( t_server *server);


#endif
