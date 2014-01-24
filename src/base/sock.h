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

#include "common.h"

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

	void (* connect)( struct MN_Socket *socket, int port);
	void (* print)(char *msg);

	int read;

}t_socket;

int 		socket_listen( t_socket *sock);
void 		socket_connect( t_socket *socket, int port);
void 		socket_disconnect( t_socket *sock);
t_socket *	socket_new( void);
void 		socket_free( t_socket *socket);


#ifdef __cplusplus
}
#endif

#endif
