#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int execute_task(const char *cmd)
{
	printf("Executing:%s\n",cmd);
	int status;
	pid_t pid;

	pid = fork();
	if(pid==-1)
		return -1;

	else if(pid==0)
	{
		const char *argv[5];
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = cmd;
		argv[3] = NULL;
		
		execv("/bin/sh",argv);
		exit(-1);
	}
	if(waitpid(pid,&status,0)==-1)
		return -1;

	else if(WIFEXITED(status))
		return WEXITSTATUS(status);

	return -1;
}

void remove_newlines(int x,char args[x][100])
{
	for(int i=0;i<x;i++)
	{
		for(int y=0;y<99;y++)
		{
			if(args[i][y]=='\n')
				args[i][y]='\0';
		}
	}
}

