/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "ctx.h"

int server_init=0;


void *__server(void *ptr);

void slave(void)
{
	t_context *C=ctx_get();
	t_app *app = C->app;

	printf("entering slave mode ...\n");

	app->clock->limit=1;
	server_connect(C->server,9901);

	while(1)
	{
		app_sleep(app);
		printf("[%d] hello\n",app->frame);
		//__server(NULL);
	}
}

void server_print(char *msg)
{
	t_context *C=ctx_get();
	t_term *term = C->term;
	term_print(term,msg);
}

void *__server(void *ptr)
{
	t_context *C=ctx_get();
	char msg[30];

	if(!server_init)
	{
		socket_connect(C->server->socket,9901);
		server_init=1;
		sprintf(msg,"listening on port %d",C->server->socket->portno);
		term_print(C->term,msg);

		if(C->app->off_screen)
		{
			printf("listening on port %d\n",C->server->socket->portno);
		}
	}
	else
	{
		socket_listen(C->server->socket);
		if(C->app->off_screen)
		{
			printf("waiting for connection");
		}

	}
	return NULL;
}

void server_connect(t_server *server,int port)
{
	t_context *C=ctx_get();

	t_process *process=process_add(C,"server",__server);
	process->clock->limit=1;
	process_launch(process);
}

t_server *server_new(void)
{
	t_server *server = (t_server *)malloc(sizeof(t_server));

	server->socket=socket_new();
	server->socket->print=server_print;

	return server;
}
