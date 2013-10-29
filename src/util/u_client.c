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
