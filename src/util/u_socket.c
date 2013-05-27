/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */
 
#include "common.h"
#include "util.h"
#include "network.h"
#include "system.h"

void socket_print(char *msg)
{
	printf("%s",msg);
}

int socket_listen(t_socket *sock)
{
	char msg[100];
	int n;
	char buffer[256];

	sock->clilen = sizeof(sock->cli_addr);

	sock->newsockfd = accept(sock->sockfd, (struct sockaddr *) &sock->cli_addr, &sock->clilen);

	if (sock->newsockfd < 0) 
	{
		sprintf(msg,"error on accept");
		sock->print(msg);
	}


	bzero(buffer,256);
	n = read(sock->newsockfd,buffer,255);
	if (n < 0)
	{
		sprintf(msg,"error reading from socket");
		sock->print(msg);
	}

	sprintf(msg,"msg %s\n",buffer);
	printf("msg %s\n",buffer);

	if(buffer[0])
		sock->print(msg);

	n = write(sock->newsockfd,"I got your message",18);

	if (n < 0)
	{
		sprintf(msg,"error writin to socket");
		sock->print(msg);
	}
	else
	{ 
		sprintf(msg,"msg sent");
		sock->print(msg);
	}
	
	sock->print(msg);

	return 1;
}

void socket_connect(t_socket *sock,int port)
{
	char msg[100];
	char hostname[128];
		
	if(sys_get_hostname(hostname))
	{
		//printf("ok<%s>\n",hostname);
	}

	sock->portno=port;

	// SOCKET
	sock->sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sock->sockfd < 0) 
	{
		sprintf(msg,"error opening socket");
		sock->print(msg);
	}

	sock->server = gethostbyname(hostname);

	if (sock->server == NULL)
	{
		sprintf(msg,"error no such host %s\n",hostname);
		sock->print(msg);
	}
	else
	{
		sock->print("server start");

		bzero((char *) &sock->serv_addr, sizeof(sock->serv_addr));
		sock->serv_addr.sin_family = AF_INET;
		sock->serv_addr.sin_addr.s_addr = INADDR_ANY;
		sock->serv_addr.sin_port = htons(sock->portno);

		// BIND
		if (bind(sock->sockfd, (struct sockaddr *) &sock->serv_addr,sizeof(sock->serv_addr)) < 0) 
		{
			sprintf(msg,"error on binding");
			sock->print(msg);
		}
		else
		{
			sprintf(msg,"binding ok");
			sock->print(msg);
		}

		// LISTEN
		listen(sock->sockfd,5);

	}
}

void socket_disconect(t_socket *sock)
{
	close(sock->newsockfd);
	close(sock->sockfd);
}

t_socket *socket_new(void)
{
	t_socket *socket=(t_socket *)malloc(sizeof(t_socket));
	socket->connect=socket_connect;
	socket->print=socket_print;

	return socket;
}
