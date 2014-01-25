/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */
 
#include "util.h"

void socket_print(const char *msg)
{
	printf("%s\n",msg);
}

void socket_read( t_socket *sock)
{
	int status;
	if( sock->accept)
	{
		bzero( sock->buffer, SOCKET_BUFFER);
		status = read( sock->newsockfd, sock->buffer, SOCKET_BUFFER);

		if( status < 0)	sock->print( "[SOCKET READ] Error reading from socket"); 
		if( status > 0)	sock->read = 1; 
	}
	else
	{
		//sock->print( "[SOCKET READ] Error no accept"); 
	}
}

int socket_write( t_socket *socket, const char *msg)
{
	int status;
	if( socket->connected)
	{
		bzero( socket->buffer, SOCKET_BUFFER);
		status = write( socket->sockfd, msg, strlen( msg));

		if (status < 0)
		{
			socket->print("[SOCKET WRITE] Error writing to socket"); 
			return 0;
		}
		else if( status == 0)
		{
			socket->print("[SOCKET WRITE] Error writing 0"); 
			return 0;
		}
		else if(status > 0)
		{
		//	socket->print("[SOCKET WRITE] Send OK");
			return 1;
		}
	}
	else
	{
		socket->print("[SOCKET WRITE] Error no connection");
		return 0;
	}

	return 0;
}

int socket_accept( t_socket *sock)
{
	sock->clilen = sizeof( sock->cli_addr);
	sock->newsockfd = accept( sock->sockfd, (struct sockaddr *) &sock->cli_addr, &sock->clilen);

	if (sock->newsockfd < 0)
	{
		sock->print("[SOCKET ACCEPT] Error on accept"); 
		return 0;
	}
	else
	{
		sock->print( "[SOCKET ACCEPT] OK");
		sock->accept = 1;
		return 1;
	}
}

int socket_init( t_socket *s, int port)
{
	int size = 64;
	char hostname[size];

	s->port = port;
		
	if( sys_get_hostname( hostname, size))
	{
		s->sockfd = socket( AF_INET, SOCK_STREAM, 0);

		if (s->sockfd < 0) s->print("[SOCKET INIT] ERROR opening socket"); 

		s->server = gethostbyname( hostname);

		if ( s->server == NULL) 
		{
			s->print("[SOCKET INIT] Error on host");
			return 0;
		}
		else
		{
			/*
			char msg[size];
			snprintf( msg, size, "[SOCKET INIT] Init on port %d", s->port);
			s->print(msg);
			*/

			bzero((char *) &s->serv_addr, sizeof(s->serv_addr));
			s->serv_addr.sin_family = AF_INET;
			s->serv_addr.sin_addr.s_addr = INADDR_ANY;
			s->serv_addr.sin_port = htons(s->port);

			return 1;
		}
	}
	else
	{
		s->print("SOCKET ERROR hostname");
		return 0;
	}
}

void socket_bind( t_socket *socket, int port)
{
	int size = 64;
	char msg[size];
	if( socket_init( socket, port))
	{
		// Bind
		if(
			bind( 	socket->sockfd,
				(struct sockaddr *) &socket->serv_addr,
				sizeof(socket->serv_addr))
			< 0) 
		{
			socket->print( "[SOCKET BIND] Error on binding\n");
		}
		else
		{
			// Listen
			listen( socket->sockfd, 5);

			snprintf( msg, size, "[SOCKET BIND] Binding on port %d", port);
			socket->binded = 1;
			socket->print( msg);

			// Accept
			socket_accept( socket);
		}

	}
}


int socket_connect( t_socket *socket, int port)
{
	if( socket_init( socket, port))
	{
		bzero((char *) &socket->serv_addr, sizeof(socket->serv_addr));
		socket->serv_addr.sin_family = AF_INET;

		bcopy(
			(char *) socket->server->h_addr, 
			(char *) &socket->serv_addr.sin_addr.s_addr,
			socket->server->h_length);

		socket->serv_addr.sin_port = htons( socket->port);

		if(
			connect(
				socket->sockfd,
				(struct sockaddr *) &socket->serv_addr,
				sizeof(socket->serv_addr))
			< 0)
		{
			socket->print("[SOCKET CONNECT] Error connecting");
			return 0;
		}
		else
		{
			socket->print("[SOCKET CONNECT] Connection OK");
			socket->connected = 1;
			return 1;
		}
	}
	else
	{
		socket->print("[SOCKET CONNECT] Error on init");
		return 0;
	}
}

void socket_unbind(t_socket *sock)
{
	close(sock->newsockfd);
	close(sock->sockfd);
}

void socket_free( t_socket *socket)
{
	mem_free( socket, sizeof( t_socket));
}

t_socket *socket_new( void)
{
	t_socket *socket = (t_socket *) mem_malloc( sizeof( t_socket));
	socket->bind = socket_bind;
	socket->print = socket_print;
	socket->read = 0;
	socket->write = 0;
	socket->connected = 0;
	socket->accept = 0;
	socket->binded = 0;

	return socket;
}


