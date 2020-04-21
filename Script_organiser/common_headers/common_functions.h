#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int execute_task(const char *cmd);
void remove_newlines(int x,char args[x][100]);
