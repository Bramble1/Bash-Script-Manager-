#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/wait.h>

/*open a file
 * ----------
 * 1-Create a fd using custom permissions
 * 2- return the file handler*/
FILE* open_file(char *file , char *permissions)
{
	/*1.*/
	FILE *fd;
	fd = fopen(file,permissions);
	if(!fd)
	{
		perror("fopen");
		exit(1);
	}
	/*2.*/
	return fd;
}

/*send a file from the server to the client
 * ----------------------------------------
 * 1- read chunks of the file and send those chunks across the network
 * 2- keep reading and sending the file until we have finished reading the file and have
 *    nothing left to send.*/
void send_file(FILE *fd,int sockfd)
{
	char *file[8000];
	int num_read;

	/*1. 2.*/
	while((num_read=fread(file,sizeof(char),sizeof(file),fd))>0)
	{
		if(send(sockfd,file,num_read,0)==-1)
		{
			perror("send");
			exit(1);
		}
	}
}

/*recv a file that is being sent from the server to the client side
 * ----------------------------------------------------------------
 * 1- repeatedly read chunks of data from socket and writing each chunk to the buffer.
 * 2- keep reading from socket until no data is being sent.*/
void recv_file(FILE *fd,int sockfd)
{
	char *file[8000];
	int num_read;
	/*1. 2.*/
	while((num_read=recv(sockfd,file,sizeof(char),0))>0)
	{
		if(fwrite(file,sizeof(char),num_read,fd)==-1)
		{
			perror("fwrite");
			exit(1);
		}
	}
}

/*Create a file containing the list of available packages the server can send.
 * ---------------------------------------------------------------------------
 * 1- Create a child process and execute the bash script to create the text file
 * containing all available .tar.gz files in the server directory.
 * 2- wait for the child process to terminate making sure it terminates correctly.*/
void CreatePackageList()
{
	pid_t pid;
	pid_t p_wait;
	int status;

	if((pid = fork())<0)
	{
		perror("fork");
		exit(1);
	}

	/*1.*/
	if(!pid)
	{
		int ret;
		ret = execl("./list_files.sh",NULL,NULL);
		if(ret<0)
		{
			perror("execl");
			exit(1);
		}
		exit(ret);

		/*2.*/
		if((p_wait = wait(&status))<0)
		{
			perror("wait");
			exit(1);
		}
		/*check to make sure it had a normal termination*/
		if(WIFEXITED(status))
		{
			printf("Normal termination with exist status=%d\n",WEXITSTATUS(status));
		}
		else
		{
			perror("process did not terminate normally.");
			exit(1);
		}
	}

	

}
