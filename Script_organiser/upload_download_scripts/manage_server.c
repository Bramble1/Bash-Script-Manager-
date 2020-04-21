#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include"network_functions/client_setup.h"
#include"network_functions/files.h"
#include"network_functions/communicate.h"
#include"../common_headers/common_functions.h"
#include"../common_headers/common_data_structures.h"
#define PORT 3490
#define MAXDATASIZE 8000
#define DIRECTORY "../scripts_repo/\0"
#define SCRIPT_LOG "../scripts_repo/scripts.txt\0"
#define SERVER_SCRIPTS "../scripts_repo/server_scripts.txt\0"

/*struct Script_info
{
	char path[100];
	int no_args;
	char description[100];
	char name[100];
};

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
}*/

void log_scripts_file(char *filename)
{
	FILE *fd = fopen(SERVER_SCRIPTS,"r");
	if(fd==NULL)
	{
		perror("fopen1:");
		exit(1);
	}
	FILE *fd2 = fopen(SCRIPT_LOG,"r");
	if(fd2==NULL)
	{
		perror("fopen2");
		exit(1);
	}
	struct Script_info script;
	int record_exists = 0;
	
	while(fread(&script,sizeof(struct Script_info),1,fd2))
	{
		if(strcmp(script.name,filename)==0)
		{
			record_exists = 1;
			break;
		}
	}
	fclose(fd2);
	if(record_exists==0)
	{
		fd2 = fopen(SCRIPT_LOG,"a");
		if(fd2==NULL)
		{
			perror("fopen3:");
			exit(1);
		}
		while(fread(&script,sizeof(struct Script_info),1,fd))
		{
			if(strcmp(script.name,filename)==0)
			{
				printf("storing file to repo record");
				fwrite(&script,sizeof(struct Script_info),1,fd2);
				break;
			}
			else
				printf("no match found\n");
		}
	}
}

void unlog_scripts_file(char *filename)
{
	struct Script_info script;
	FILE *fd = fopen(SERVER_SCRIPTS,"r");
	if(fd==NULL)
	{
		perror("unlog_scripts_file():fopen1:");
		exit(1);
	}
	FILE *fd2 = fopen("temp.txt\0","a");
	if(fd2==NULL)
	{
		perror("unlog_scripts_file():fopen2:");
		exit(1);
	}

	while(fread(&script,sizeof(struct Script_info),1,fd))
	{
		if(strcmp(script.name,filename)!=0)
		{
			fwrite(&script,sizeof(struct Script_info),1,fd2);
		}
	}
	fclose(fd); fclose(fd2);
	remove(SERVER_SCRIPTS);

	fd2 = fopen("temp.txt\0","r");
	if(fd2==NULL)
	{
		perror("open");
		exit(1);
	}
	fd = fopen(SERVER_SCRIPTS,"a");
	if(fd==NULL)
	{
		perror("fopen");
		exit(1);
	}

	while(fread(&script,sizeof(struct Script_info),1,fd2))
	{
		fwrite(&script,sizeof(struct Script_info),1,fd);
	}
	
	fclose(fd2); fclose(fd);
	remove("temp.txt\0");
}

void log_server_scripts(char *name)
{
	int record_exists = 0;
	struct Script_info script;
	
	FILE *fd = fopen(SERVER_SCRIPTS,"r");
	if(fd==NULL)
	{
		perror("log_server_scripts():fopen1:");
	}
	else
	{
		while(fread(&script,sizeof(struct Script_info),1,fd))
		{
			if(strcmp(script.name,name)==0)
			{
					record_exists = 1;
					break;
			}
		}
		fclose(fd);
	}
	if(record_exists==0)
	{
		FILE *fd = fopen(SERVER_SCRIPTS,"a");
		if(fd==NULL)
		{
			perror("log_server_scripts():fopen2:");
			exit(1);
		}
		FILE *fd2 = fopen(SCRIPT_LOG,"r");
		if(fd2==NULL)
		{
			perror("log_server_scripts():fopen:");
			exit(1);
		}

		while(fread(&script,sizeof(struct Script_info),1,fd2))
		{
			if(strcmp(script.name,name)==0)
			{
				fwrite(&script,sizeof(struct Script_info),1,fd);
				break;
			}
		}
		fclose(fd); fclose(fd2);
	}
}


void upload_file(int sockfd, char *file)
{
	char location[80];
	char package[80];
	strcpy(location,DIRECTORY);
	location[strlen(DIRECTORY)]='\0';

//	sanitize_string(file);

	strcpy(package,file);
	package[strlen(file)]='\0';
//	strcat(package,".sh");

	char command[100];
	strcpy(command,upload_package);
	strcat(command," \0");
	strcat(command,package);

	printf("sending command:%s\n",command);

	if((send(sockfd,command,strlen(command),0))==-1)
	{
		perror("send");
		exit(1);
	}
	sleep(2);

	strcat(location,package);
	FILE *fd = fopen(location,"r");
	if(fd==NULL)
	{
		perror("upload_file():fopen:");
		exit(1);
	}
	send_file(fd,sockfd);
	fclose(fd);
	printf("\npackage = %s\n",package);
	log_server_scripts(package);	
}


void retrieve_file(int sockfd, char *file,char *directory)
{
	char buf[MAXDATASIZE];
	char package[50];
	char location[50];
	FILE *fd;

	strcpy(location,directory);
	location[strlen(directory)]='\0';

//	sanitize_string(file);

	strcpy(package,file);
	package[strlen(file)]='\0';
//	strcat(package,".sh");

	char command[50];
	strcpy(command,sendpackage);
	strcat(command," \0");
	strcat(command,package);


	if((send(sockfd,command,strlen(command),0))==-1)
	{
		perror("send");
		exit(1);
	}



	strcat(location,package);
	printf("saving file to:%s\n",location);
	fd = open_file(location,"w");
	recv_file(fd,sockfd);
	fclose(fd);
	log_scripts_file(package);
	chmod(location,S_IXUSR); 
}

void query_server_files(struct Script_info *script)
{
	FILE *fd = fopen(SERVER_SCRIPTS,"r");
	if(fd==NULL)
	{
		perror("fopen:");
		exit(1);
	}
	
	while(fread(script,sizeof(struct Script_info),1,fd))
	{
		printf("Name:%s Args:%d\nDescription:%s\n",script->name,script->no_args,script->description);
		printf("-------------------------------\n");
	}
	fclose(fd);
}

void delete_file(int sockfd,char *file)
{
	struct Script_info script;
	int file_exists = 0;
	char package[50];
	strcpy(package,file);
	package[strlen(file)] = '\0';
	strcat(package,".sh");
	printf("package = %s\n",package);

	FILE *fd = fopen(SERVER_SCRIPTS,"r");
	if(fd==NULL)
	{
		perror("fopen:");
	}
	else
	{
		while(fread(&script,sizeof(struct Script_info),1,fd))
		{
			if(strcmp(script.name,package)==0)
			{
				file_exists = 1;
				printf("file requested for deletion exists\n");
				break;
			}
		}
		fclose(fd);
	}
	if(file_exists==1)
	{
		char buf[MAXDATASIZE];
//		char location[50];
		FILE *fd;

//		strcpy(location,directory);
//		location[strlen(directory)]='\0';
		
		sanitize_string(file);

		char command[50];
		strcpy(command,delete_package);
		strcat(command," \0");
		strcat(command,package);

		printf("sending command:%s\n",command);

		if(send(sockfd,command,strlen(command),0)==-1)
		{
			perror("send");
			exit(1);
		}
		unlog_scripts_file(package);
	}

}

int main()
{
	int sockfd;
	struct Script_info script;
	char inputs[3][100];
	printf("Upload(u),download(d),query(q),remove(r):");
	fgets(inputs[0],99,stdin);
	remove_newlines(1,inputs);


	if(strcmp(inputs[0],"u\0")==0 || strcmp(inputs[0],"d\0")==0 || strcmp(inputs[0],"r\0") ==0)
	{
		printf("Enter hostname:");
		fgets(inputs[1],99,stdin);
		printf("Enter filename:");
		fgets(inputs[2],99,stdin);
		remove_newlines(3,inputs);			
	
		sockfd = setup_client_socket();
		connect_to_address(sockfd,inputs[1],3491);
	}

	if(strcmp(inputs[0],"u\0")==0)
	{
		printf("user wants to upload file \n");
		upload_file(sockfd,inputs[2]);
	
	}
	else if(strcmp(inputs[0],"d\0")==0)
	{
		printf("user wants to download file:%s\n",inputs[2]);
		retrieve_file(sockfd,inputs[2],DIRECTORY);
	}
	else if(strcmp(inputs[0],"r\0")==0)
	{
		printf("user wants to delete file\n");
		delete_file(sockfd,inputs[2]);
	}
	else if(strcmp(inputs[0],"q\0")==0)
	{
		printf("user wants to query.");
		query_server_files(&script);
	}

	
	return 0;

	
}
