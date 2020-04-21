#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"../execution_path.h"
#include"../common_headers/common_functions.h"
#include"../common_headers/common_data_structures.h"
#define REPO "../scripts_repo/\0"
#define SCRIPT_LOG "../scripts_repo/scripts.txt\0"
#define SCHEDULED_SCRIPT_LOG "../scripts_repo/scheduled_scripts.txt\0"
#define SIZE 2048
#define COPY "../scripts_repo/scheduled_scripts2.txt\0"
/*script info*/
/*struct script_info
{
	char path[100];
//	char *path;
	int no_args;
	char description[100];
	char name[100];
}; */

struct Schedule
{
	char execution_string[100];
	char filename[100];
	//char *execution_string;
	int hour;
	int minute;
	int weekday;
	int executed;
};
int schedule = 0;

void schedule_task(struct Schedule *task)
{
	/*1. write schedule struct task to scheduled_tasks.txt , this will be read by the daemon to execute*/
	FILE *fd = fopen(SCHEDULED_SCRIPT_LOG,"a");
	if(fd==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);
	}
	task->executed = 0;
	fwrite(task,sizeof(struct Schedule),1,fd);
	if(fwrite !=0)
		printf("contents written sucessfully\n");
	else
	{
		perror("fwrite:");
		exit(EXIT_FAILURE);
	}

	fclose(fd);	
}
void view_scheduled_tasks(struct Schedule *task)
{
	FILE *fd = fopen(SCHEDULED_SCRIPT_LOG,"r");
	if(fd==NULL)
	{
		perror("fopen:");
		exit(1);
	}
	printf("\n");
	while(fread(task,sizeof(struct Schedule),1,fd))
	{
		printf("task.name:%s\n",task->filename);
		printf("task.hour:%d\n",task->hour);
		printf("task.minute:%d\n",task->minute);
		printf("task.weekday:%d\n",task->weekday);
		printf("\n");
	}
	fclose(fd);
}

/*int execute_task(char *command)
{
	printf("command = %s\n",command);
	int status;
	pid_t pid;

	pid = fork();
	if(pid==-1)
		return -1;
	else if(pid==0)
	{
		const char *args[4];
		
		args[0] = "sh";
		args[1] = "-c";
		args[2] = command;
		args[3] = NULL;
		execv("/bin/sh",args);

		exit(-1);
		
	}

	if(waitpid(pid,&status,0)==-1)
		return -1;
	else if(WIFEXITED(status))
		return WEXITSTATUS(status);

	return -1;		
}*/
void create_command(char *filename ,int x, char args[x][100],char command[100])
{
	for(int i=0;i<x;i++)
	{
		for(int y=0;y<99;y++)
		{
			if(args[i][y]=='\n')
				args[i][y]='\0';
		}
	}

	char cmds[100];
//	strcpy(cmds,"/home/samdeep/Projectv10/scripts_repo/\0");
	strcpy(cmds,REPO_PATH);
	strcat(cmds,filename);
	strcat(cmds," \0");
	for(int i=0;i<x;i++)
	{
		strcat(cmds,args[i]);
		strcat(cmds," \0");
	}
	strcpy(command,cmds);		
//	char *cmd = cmds;
//	return cmd;
}

void get_arguments(char *argv[],struct Schedule *task,struct Script_info *script)
{

	
	int hour , minute , weekday;
	hour = minute = weekday = 0;

	if(script->no_args==0)
	{
	
		if(strcmp(argv[2],"e")==0){ 
		
		/*add the filename to execution path*/
		char buf[SIZE]; 
		strcpy(buf,REPO);
		strcat(buf,"./\0");
		strcat(buf,script->name);
	//	char *path = buf;
		strcpy(script->path,buf);
		}
	
		if(strcmp(argv[2],"s")==0)
		{


		
		

			 hour = 2+script->no_args + 1;
			 minute = hour + 1;
			 weekday = minute+1;
			

			char buf[SIZE];
			strcpy(buf,REPO);
			strcat(buf,"./\0");
			strcat(buf,script->name);
			char *path = buf;

			
			task->hour = atoi(argv[hour]);
			task->minute = atoi(argv[minute]);
			task->weekday = atoi(argv[weekday]); 
			strcpy(task->execution_string,buf);
		}

		
	}
	else
	{	
		int start_pos,firstval;
		start_pos = 3; firstval = 0;
		char buf2[SIZE];

	
	
		for(int i=0;i<script->no_args;i++)
		{
			printf("args = %s\n",argv[start_pos+i]);
			if(firstval==0)
			{
				strcpy(buf2,argv[start_pos+i]);
				buf2[strlen(buf2)]='\0';
				strcat(buf2," \0");
				firstval=1;
			}
			else
			{
				strcat(buf2,argv[start_pos+i]);
				buf2[strlen(buf2)]='\0';
				strcat(buf2," \0");
			}
		}

	

			char file_path[SIZE];
			strcpy(file_path,REPO);
			strcat(file_path,"./\0");
			strcat(file_path,script->name);
			file_path[strlen(file_path)] = '\0';
			strcat(file_path," \0");
			strcat(file_path,buf2);
			char *path = file_path;

			if(strcmp(argv[2],"e")==0)
			{
				strcpy(script->path,file_path);
			}
			else if(strcmp(argv[2],"s")==0)
			{
				hour = 2 + script->no_args + 1;
				minute = hour + 1;
				weekday = minute + 1;

				task->hour = atoi(argv[hour]);
				task->minute = atoi(argv[minute]);
				task->weekday = atoi(argv[weekday]);

				schedule = 1;
				strcpy(task->execution_string,file_path);
				 
			}
	
	}	
}

void obtain_script_information(char *script_name,struct Script_info *script)
{
	/*open log*/
	FILE *file;
	char buf[SIZE];
	size_t bytes;

	file = fopen(SCRIPT_LOG,"r");
	if(file==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);
	}

	while(fread(script,sizeof(struct Script_info),1,file))
	{
		if(strcmp(script->name,script_name)==0)
			break;
	}

	fclose(file);
}

int main(int argc,char *argv[])
{
	/*1. get script_name from user
	  2. Find out no of arguments
	  3. we then access argv[no_arguments] until we reach no_args
	  4. then fork and exec bash shell executing the script with the arguments
	     supplied by the user, by strcating into one string seperated by spaces*/

	//can schedule scripts which require user input to be executed within this program, or any daemons to be scheduled that
	//dont require user input can be sent to the scheduler daemon. if user enters s as an argument, then we call scheduling
	//function which logs the task struct to the scheduled_list for the daemon to then check and execute, it will be a struct
	//containing the execution_path and hour , min and weekday to execute.	
	

	

	struct Schedule task;
	struct Script_info script;
//	printf("argv[2] = %s\n",argv[2]);
	char inputs[6][100];
	printf("Execute(e)/Schedule(s)/query_scheduled_tasks(q):");
	fgets(inputs[0],99,stdin);
	
	if(strcmp(inputs[0],"e\n")==0)
	{
		printf("Enter filename:");
		fgets(inputs[1],99,stdin);
/*		for(int i=0;i<99;i++)
		{
			if(inputs[1][i]=='\n'){
				printf("found a newline macro in %s\n",inputs[1]);
				inputs[1][i]='\0';
				break;}
		}*/
		remove_newlines(2,inputs);
		obtain_script_information(inputs[1],&script);
//		obtain_script_information("print.sh\0",&script);
		printf("user wants to execute script:%s\n",script.name);
		char file_arguments[script.no_args][100];
		for(int i=0;i<script.no_args;i++)
		{
			printf("Enter %s arg:",script.name);
			fgets(file_arguments[i],99,stdin);
			
		}
		char command[100];
	//	remove_newlines(script.no_args,file_arguments);
		create_command(script.name,script.no_args,file_arguments,command);
		printf("command to execute = %s\n",command);
		execute_task(command);
		
	}
	else if(strcmp(inputs[0],"s\n")==0)
	{
		printf("Enter filename:");
		fgets(inputs[1],99,stdin);
		remove_newlines(2,inputs);
		obtain_script_information(inputs[1],&script);
		printf("User wants to execute script:%s\n",script.name);
		char file_arguments[script.no_args][100];
		for(int i=0;i<script.no_args;i++)
		{
			printf("Enter %s arg:",script.name);
			fgets(file_arguments[i],99,stdin);
		}

		printf("Enter hour:");
		fgets(inputs[2],99,stdin);
		printf("Enter minute:");
		fgets(inputs[3],99,stdin);
		printf("Enter weekday:");
		fgets(inputs[4],99,stdin);

		remove_newlines(script.no_args,file_arguments);
		remove_newlines(5,inputs);
		char command[100];
		create_command(script.name,script.no_args,file_arguments,command);
		
		strcpy(task.execution_string,command);
		task.hour = atoi(inputs[2]);
		task.minute = atoi(inputs[3]);
		task.weekday = atoi(inputs[4]);	
		strcpy(task.filename,script.name);
	
		printf("\n");
		printf("scheduling\n");
		printf("script.name=%s\n",task.filename);
		printf("script.path=%s\n",task.execution_string);
		printf("script.hour=%d\n",task.hour);
		printf("script.minute=%d\n",task.minute);
		printf("script.weekday=%d\n",task.weekday);
		schedule_task(&task);	
	}
	else if(strcmp(inputs[0],"q\n")==0)
	{
//		printf("user wants to query scheduled tasks.\n");
		view_scheduled_tasks(&task);
	}

/*	if(argc==1)
	{
		printf("./execute scriptname e <no_args>\n");
		printf("./execute scriptname s <no_args> hour minute weekday\n");
		exit(1);
	} */
	
	//check the first argument to see if it's not a filename if not, then it must be something else
	

/*	char path[60];
	obtain_script_information(argv[1],&script);
	get_arguments(argv,&task,&script);
	if(schedule==1)
	{
		printf("user wants to schedule a task here is the info:\n");
		printf("task.execution_string = %s\n",task.execution_string);
		printf("task.hour = %d ; task.min = %d ; task.weekday = %d\n",task.hour,task.minute,task.weekday);
		schedule_task(&task);
	}
	else	
	{
		printf("user wants to execute a task here is the info:\n");
		printf("script.path = %s\n",script.path);
		execute_task(script.path);
	}

*/	

//	printf("execution path for script = %s\n",path);
//	execute_task(path);	
	
}
	

