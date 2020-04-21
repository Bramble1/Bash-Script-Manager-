#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "network_functions/communicate.h"
#include "network_functions/server_setup.h"
#include "network_functions/files.h"
#define MYPORT 3490
#define BACKLOG 10
#define MAXDATASIZE 8000
//#define DIRECTORY "/home/samdeep/University/3rd_year_project/download_upload_scripts/\0"
#define SERVER_REPO "Server_repo/\0"
/*wait for all the child processes thatthe server creates for each client*/
void sigchld_handler(int s)
{
	while(wait(NULL) >0);
}
/*how the client program interacts with the server here. The main logic of the server program.*/
void talk(int sockfd)
{
	FILE *fd;
	int error;
	char buf[MAXDATASIZE];
	
	/*recieve input*/
	if((recv(sockfd,buf,MAXDATASIZE-1,0))==-1)
	{
		perror("recv");
		exit(1);
	}

	/*check what fuctionality the client wants*/

	/*break the command down to check what command it is and the arguments*/
	char brkn_command[3][20];
	char *command = buf;
	char (*instruction)[20] = break_string(brkn_command,command);

	printf("recieved command: %s and %s\n",instruction[0],instruction[1]);


	/*interpret instructions sent from client*/

	/*client request: send a package.*/
	if((strcmp(instruction[0],sendpackage))==0)
	{
		/*attempt to open the file client wants. if failed, then send an invalidPackage message to client. Else
		 * open file and read file and write to socket to the client.*/
		char repo[100];
		strcpy(repo,SERVER_REPO);
		strcat(repo,instruction[1]);
		printf("client wants this file : %s\n",repo);
		fd = fopen(repo,"r");
		if(!fd)
		{
			perror("fopen:");
		}
		else
		{
			printf("sending package exists message.\n");
			send_file(fd,sockfd);
			fclose(fd);
		}
	} /*query request*/
	else if((strcmp(instruction[0],query))==0)
	{
		CreatePackageList();
		sleep(1);
		FILE *newfd;
	    newfd =	open_file("packages.txt","r");
		send_file(newfd,sockfd);
	}
	/*file upload request*/
	else if((strcmp(instruction[0],upload_package))==0)
	{
		/*
		char file[80];
		strcpy(file,DIRECTORY);
		strcat(file,instruction[1]);
		printf("creating new file in: %s\n",file);
		//open the file name then reacv and write file to hdd
		FILE *fd = fopen(instruction[1],"w+");
		if(fd == NULL)
		{
			perror("fopen:");
			printf("trying to open: %s\n",instruction[1]);
			exit(1);
		}

		recv_file(fd,sockfd);
		fclose(fd); */

		printf("recieved command %s and %s\n",instruction[0],instruction[1]);

		/*open the file when we are done, send ready_to_upload command to client*/		  /*once the client recieves this,the server will call recv_file and thw
		 client will send_file.*/
		char repo[100];
		strcpy(repo,SERVER_REPO);
		strcat(repo,instruction[1]);
		printf("uploading file to : %s\n",repo);

		FILE *fd = fopen(repo,"w+");
		if(fd == NULL)
		{
			perror("fopen:");
			exit(1);
		}

		recv_file(fd,sockfd); 
	}
	else if((strcmp(instruction[0],delete_package))==0)
	{
		char repo[100];
		strcpy(repo,SERVER_REPO);
		strcat(repo,instruction[1]);
		
		//attempt a remove() syscall, but first have to 
		printf("recieved command %s and %s\n",instruction[0],instruction[1]);
		printf("deleting file : %s\n");
		remove(repo);
	}

}


int main(void)
{

	int sockfd;
	struct sigaction sa;

	/*setup server settings, address , socket etc.*/
	sockfd = setup_server_socket(3491);
	Listen_for_connections(sockfd);
	
	/*this handles any dead processes from client and server iteractions which are complete, so we wait and reap all the child processes.*/
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD,&sa,NULL)==-1)
	{
		perror("sigaction");
		exit(1);
	}

	/*wait for a connection, once we have a connection accept client request and talk() to the client.*/
	accept_client_connection(sockfd,talk);
	close(sockfd);

	return 0;



}
