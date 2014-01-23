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
#include "op.h"
#include "app.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "camera.h"
#include "ui.h"
#include "file.h"
#include "server.h"
#include "term.h"
#include "process.h"
#include "list.h"
#include "block.h"
#include "brick.h"
#include "clock.h"
#include "engine.h"

void slave(void)
{
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
}

void server_print(char *msg)
{
	t_context *C=ctx_get();
	t_term *term = C->term;
	term_print(term,msg);
}

void *server_daemon( void *ptr)
{
	t_context *C=ctx_get();
	t_process *process = (t_process *) ptr;
	t_server *server = process->data;

	char msg[30];

	if( !server->init)
	{
		server->init = 1;
		socket_connect( server->socket, 9901);
		sprintf(msg,"listening on port %d", server->socket->port);
		term_print(C->term,msg);

		if(C->app->off_screen)
		{
			printf("listening on port %d\n",C->server->socket->port);
		}
	}
	else
	{
		if( !server->socket->read)
		{
			socket_listen( server->socket);

			if(C->app->off_screen)
			{
				printf("waiting for connection");
			}
		}
	}

	return NULL;
}

void server_start( t_server *server, int port)
{
	t_context *C=ctx_get();

	t_process *process = process_add( C, "server", server_daemon);
	process->clock->limit = 1;
	server->process = process;
	process->data = server;
	process_launch( process);
}

void server_stop( t_server *server)
{
	t_context *C=ctx_get();
	socket_disconnect( server->socket);
	engine_process_remove( C->engine, server->process);
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

	return server;
}
