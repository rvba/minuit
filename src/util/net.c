/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */
 
#include "util.h"

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
	t_socket *socket=(t_socket *)mem_malloc(sizeof(t_socket));
	socket->connect=socket_connect;
	socket->print=socket_print;

	return socket;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int tcp_client(void)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

	portno=9901;
	//char hostname[]="eeepc";
	char hostname[128];
	if(sys_get_hostname(hostname))
	{
	}

    char buffer[256];
	/*
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	*/
    //portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    //server = gethostbyname(argv[1]);
    printf("tcp_lient:hostname:%s\n",hostname);
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    if(fgets(buffer,255,stdin) != NULL)
    {
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    }
    close(sockfd);
    return 0;
}

