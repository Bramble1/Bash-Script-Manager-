#include"client_setup.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define PORT 3490

/*Creating a tcp socket to be used on the client side
 * --------------------------------------------------
 *  1-create a tcp socket using AF_INET which determines what
 *  type of address we want to use: ipv4 and sock_stream being TCP protocol
 *  2- return the socket.*/
int setup_client_socket()
{
	int sockfd;
	/*1.*/
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	/*2.*/
	return sockfd;	
}

/*Using the created socket to connect to the server address and port
 * -----------------------------------------------------------------
 *  1- convert the address to an ip address
 *  2- populate their_addr struct with server information
 *  3- using the socket and their_addr struct to connect to the server*/
void connect_to_address(int sockfd,char *address,int port)
{
	/*1.*/
	struct hostent *he;
	if((he=gethostbyname(address))==NULL)
	{
		perror("gethostbyname");
		exit(1);
	}

	/*2.*/
	struct sockaddr_in their_addr;
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero),8);

	/*3.*/
	if(connect(sockfd,(struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		exit(1);
	}
}
