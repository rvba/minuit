/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __NETWORK_H
#define __NETWORK_H

#include "common.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>

#define SOCKET_BUFFER 2024

typedef struct MN_Socket t_socket;

// SOCKET

struct MN_Socket
{
	int socket;
	int sockfd;
	int newsockfd;
	int portno;
	char buffer[SOCKET_BUFFER];

	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	struct hostent *server;
	socklen_t clilen;

	void (* connect)(t_socket *socket,int port);
	void (* print)(char *msg);
};

int socket_listen(t_socket *sock);
void socket_connect(t_socket *socket,int port);
t_socket *socket_new(void);
void slave(void);
int tcp_client(void);
int net_connect(void);

#endif

