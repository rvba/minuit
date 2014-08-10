/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

void slave(void)
{
	/*
	t_context *C=ctx_get();
	t_app *app = C->app;

	printf("entering slave mode ...\n");

	app->clock->limit=1;
	server_start( C->server, 9901);

	while(1)
	{
		app_sleep(app);
		printf("[%d] hello\n",app->frame);
	}
	*/
}

void server_print(const char *msg)
{
	/*
	t_context *C=ctx_get();
	t_term *term = C->term;
	term_print(term,msg);
	*/
}

void *server_daemon( void *ptr)
{
	t_process *process = (t_process *) ptr;
	t_server *server = (t_server *) process->data;

	if( !server->init)
	{
		if( server->binded)
		{
			// Bind & Listen
			socket_bind( server->socket, server->port);
			server->init = 1;
		}
		else
		{
			// Connect
			socket_connect( server->socket, server->port);
			server->init = 1;
		}
	}

	return NULL;
}

void server_start( t_server *server, t_engine *engine, int port)
{
	t_process *process = process_add( engine, "server", server_daemon);
	process->clock->limit = 1;
	server->port = port;
	server->process = process;
	process->data = server;
	process_launch( process);
}

void server_stop( t_server *server, t_engine *engine)
{
	socket_unbind( server->socket);
	engine_process_remove( engine, server->process);
}

void server_free( t_server *server)
{
	socket_free( server->socket);
	mem_free( server, sizeof( t_server));
}

t_server *server_new( const char *name)
{
	t_server *server = (t_server *)mem_malloc(sizeof(t_server));

	id_init( &server->id, name);

	server->socket = socket_new();
	server->socket->print = server_print;
	server->init = 0;
	server->process = NULL;
	server->port = 0;
	server->binded = 0;

	return server;
}
