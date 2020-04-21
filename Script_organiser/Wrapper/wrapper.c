#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../common_headers/common_data_structures.h"
#include"../common_headers/common_functions.h"

void manage_repo()
{
	execute_task("./manage_repo");
}
void execute_schedule()
{
	execute_task("./execute");
}
void launch_schedule_daemon()
{
//	execute_task("./schedule");
	system("./schedule");
}
void manage_server()
{
	execute_task("./manage_server");
}

int main()
{

	system("sudo ./schedule");
	char input[1][100];
	printf("\t---Bash Script manager---");
	printf("<enter 'h' for help>\n");
	printf("\n");


	while(1){	
	printf(">: ");
	fgets(input[0],99,stdin); 

	remove_newlines(1,input);

	if(strcmp(input[0],"h\0")==0)
	{
		printf("1 - manage_repo\n");
		printf("2 - execute/schedule script\n");
		printf("3 - manage_server\n");
	}

	if(strcmp(input[0],"1\0")==0)
	{
		manage_repo();
	}
	else if(strcmp(input[0],"2\0")==0)
	{
		execute_schedule();
	}
	else if(strcmp(input[0],"3\0")==0)
	{
		manage_server();
	}

	}

	return 0;
}
