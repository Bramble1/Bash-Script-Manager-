#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#define FIFO_LOCATION "/tmp/fifo"
void Create_pipe(mode_t permissions);
void send_input(char *buf);
void recv_input(char *buf);
void remove_fifo();
