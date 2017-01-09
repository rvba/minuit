/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SOCK_H
#define __SOCK_H

#include "base.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOCKET_BUFFER 2024

typedef struct MN_Socket
{
	int socket;
	int sockfd;
	int newsockfd;
	int port;
	char buffer[SOCKET_BUFFER];

	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	struct hostent *server;
	socklen_t clilen;

	void (* bind)( struct MN_Socket *socket, int port);
	void (* print)(const char *msg);

	int read;
	int write;
	int connected;
	int accept;
	int binded;

}t_socket;

void 		socket_read( t_socket *sock);
int 		socket_write( t_socket *socket, const char *msg);
int 		socket_accept( t_socket *sock);
int 		socket_connect( t_socket *socket, int port);
void 		socket_bind( t_socket *socket, int port);
void 		socket_unbind( t_socket *sock);
t_socket *	socket_new( void);
void 		socket_free( t_socket *socket);


#ifdef __cplusplus
}
#endif

#endif
