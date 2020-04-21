#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pipes.h"
#include <stdlib.h>
#define FIFO_LOCATION "/tmp/fifo"

/*Create a fifo pipe file for interprocess communication between two unrelated processes
 * -------------------------------------------------------------------------------------
 * 1- make fifo at file_location and with certain acccess permissions.
 * 2- if mkfifo fails then print error and exit*/
void Create_pipe(mode_t permissions)
{
	//1.
	if((mkfifo(FIFO_LOCATION,permissions))==-1)
	{	//2.
		perror("mkfifo");
		exit(1);
	}

}
/*open the fifo*/
int open_fifo()
{
	int fd;
	if((fd = open(FIFO_LOCATION,O_RDWR))==-1)
	{
		perror("fopen");
		exit(1);
	}

	return fd;
}
/*close the fifo*/
int close_fifo(int fd)
{
	if((close(fd)==-1))
	{
		perror("close");
		exit(1);
	}
}




/*Open the fifo file and write string
 * ----------------------------------
 * 1-open the fifo in write only mode.
 * 2-write buf to the fifo file.
 * 3-close file descriptor.*/
void send_input(char *buf)
{
	int fd;
	//1.
	if((fd = open(FIFO_LOCATION,O_WRONLY))==-1)
	{	//2.
		perror("fopen");
		exit(1);
	}//3.
	if((write(fd,buf,strlen(buf)+1))==-1)
	{
		perror("fopen");
	}//4.
	close(fd);
}

/*read the file and retrieve sent string.
 * --------------------------------------
 * 1-open the fifo in read only mode
 * 2-read 100 bytes from fifo into buf
 * 3-close the file descriptor*/
void recv_input(char *buf)
{
	int fd;
	//1.
	if((fd = open(FIFO_LOCATION,O_RDONLY))==-1)
	{
		perror("fopen");
		exit(1);
	}
	//2.
	if((read(fd,buf,100))==-1)
	{
		perror("read");
		exit(1);
	}//3.
	close(fd);

}

/*Remove any pre-existing fifo pipe before allowing the processes to create one and communicate
 * --------------------------------------------------------------------------------------------
 *  1-use unlink() to remove the fifo*.*/
void remove_fifo()
{
	//1.
	if((unlink(FIFO_LOCATION))==-1)
	{
		perror("unlink");
	//	exit(1);
	}
}
