#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include "../common_headers/common_functions.h"
#include "../common_headers/common_data_structures.h"
//#define REPO "/home/samdeep/Projectv2/scripts_repo/\0" 
//#define SCRIPT_LOG "/home/samdeep/Projectv2/scripts_repo/scripts.txt\0"
#define REPO "../scripts_repo/\0"
#define SCRIPT_LOG "../scripts_repo/scripts.txt\0"
#define SIZE 2048


/*extracting the filename so we know what to save the copied version of the file as
 when we use the copy function to add the script to the repo*/
void extract_filename(char *buf,struct Script_info *script)
{
	char file_name[50];
	int slash_counter , filename_pos,new_counter,cnt;
	slash_counter = filename_pos = new_counter = cnt = 0;

	/*iterate through buf to get the filename*/
	for(int i = 0; i<strlen(buf); i++)
	{
		if(buf[i] == '/')
			slash_counter++;
	}

	//check to see if the file to be copied is a local one, thus no need
	// extract the filename, as the filename has just been presented, not
	//the full path just the name.
	//dealing with a fullpath
	if(slash_counter !=0)
	{
		for(int i=0; i<strlen(buf);i++)
		{
			if(buf[i] =='/')
				new_counter++;
			if(new_counter == slash_counter)
			{
				file_name[cnt] = buf[i+1];
				cnt++;
			}
		}
		
		file_name[cnt+1] = '\0';
		strcpy(script->path,REPO);
		strcat(script->path,file_name);
		strcpy(script->name,file_name);

		printf("script.path = %s\n",script->path);
		printf("script.name = %s\n",script->name);
		

	} else
		{
			strcpy(file_name,buf);
			strcpy(script->path,REPO);
			strcat(script->path,file_name);
			strcpy(script->name,file_name);

			printf("script.path = %s\n",script->path);
			printf("script.name - %s\n",script->name);
			
		}
}

void log_script(struct Script_info *script)
{
	/*open a file and write the script struct appending it to a scripts.txt file in the repo
	 - this will then be used when the admin wants a query script/s*/

	//open tasks.txt
	FILE *fd  = fopen(SCRIPT_LOG,"a");
	if(fd == NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);  
	}

	//write struct to file
	fwrite(script,sizeof(struct Script_info),1,fd);
	if(fwrite !=0)
		printf("contents written succesfully");
	else
		printf("error writing file\n");

	//close file
	fclose(fd);
		
}
void unlog_script(char *src,struct Script_info *script)
{
	FILE *fd = fopen(SCRIPT_LOG,"r");
	if(fd==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);
	}	
	
	FILE *fd2 = fopen("tmp.txt\0","a+");
	if(fd2==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);
	}
	
	while(fread(script,sizeof(struct Script_info),1,fd))
	{
		printf("script.name = %s\n",script->name);
		if(strcmp(script->name,src)!=0)
		{
			  printf("script.name = %s\n",script->name);
			  fwrite(script,sizeof(struct Script_info),1,fd2);
		}
	}

	fclose(fd);
	rewind(fd2);
	remove(SCRIPT_LOG);
	
	fd = fopen(SCRIPT_LOG,"a");
	if(fd==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE); 
	}

	while(fread(script,sizeof(struct Script_info),1,fd2))
	{
		fwrite(script,sizeof(struct Script_info),1,fd);
	}

	fclose(fd);
	fclose(fd2);
	remove("tmp.txt\0");
	
}

void copytorepo(char *src,struct Script_info *script)
{
	/*open the file to copy to the repo*/
	FILE *file, *file2;
	char buf[SIZE];
	size_t bytes;

	printf("file to copy:%s\n",src);
	file = fopen(src,"r");
	if(file==NULL)
	{
		printf("failed opening %s\n",src);
		perror("fopen error (file):");
		exit(EXIT_FAILURE);
	} 

	//open new file as thats what the name of the file will be
	printf("saving file to :%s\n",script->path);
	file2 = fopen(script->path,"w+");
	if(file2==NULL)
	{
		perror("fopen error (file2):");
		exit(EXIT_FAILURE);
	} 
	

	printf("script.path inside copytorepo() = %s\n",script->path);

	//read the file into memory
	while (0<(bytes = fread(buf,1,sizeof(buf),file)))
	{
		fwrite(buf,1,bytes,file2);
	//	fwrite(buf,1,bytes,stdout); 	
	}  
 
	fclose(file);
	fclose(file2);
//	log_script(script);	
	chmod(script->path,S_IXUSR);
	

}

void removefromrepo(char *src,struct Script_info *script)
{
	
	unlog_script(src,script);
	char buf[104];
	strcpy(buf,REPO);
	strcat(buf,src);
	printf("file to removed from directory = %s\n",buf);
	remove(buf);
}

void query_repo(char *buff,struct Script_info *script)
{
	FILE *file;
	char buf[SIZE];
	size_t bytes;

	file = fopen(SCRIPT_LOG,"r");
	if(file==NULL)
	{
		perror("fopen error:");
		exit(EXIT_FAILURE);
	}
	if(strcmp(buff,"a\0")==0)
	{
	
		while(fread(script,sizeof(struct Script_info),1,file))
		{
			printf("\n");
			printf("Name=%s\n",script->name);
			printf("args=%d\n",script->no_args);
			printf("description=%s\n",script->description);
			printf("\n");
		}
	}
	else
	{
		while(fread(script,sizeof(struct Script_info),1,file))
		{
			if(strcmp(buff,script->name)==0){
				printf("\n");
				printf("Name=%s\n",script->name);
				printf("args=%d\n",script->no_args);
				printf("description=%s\n",script->description);
				printf("\n");
				break;
			}
		}
	}

	fclose(file);
/*
	if(strcmp(buff,"a")==0)
	{
		while(fread(script,sizeof(struct Script_info),1,file))
		{
			 printf("Name=%s Args=%d\nDescription=%s\n\n",script->name,script->no_args,script->description);
			 printf("-------------------------------------|\n");
		}
	} else
		{
			while(fread(script,sizeof(struct Script_info),1,file))
			{
				if(strcmp(buff,script->name)==0)
					break;
			}
			printf("Name=%s Args = %d\nDescription=%s\n",script->name,script->no_args,script->description);
			printf("-----------------------------------|\n");
		}
	fclose(file);
	*/
} 
/*void remove_newlines(int x,char args[x][100])
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

int main(int argc, char *argv[])
{


	struct Script_info script;
	//checking arguments
	char inputs[4][100];
	printf("Add(a)/remove(d)/query(q):");
	fgets(inputs[0],99,stdin);
	
	if(strcmp(inputs[0],"q\n")==0)
	{
		printf("Enter filename or (a) for all:");
		fgets(inputs[1],99,stdin);
		remove_newlines(2,inputs);
		query_repo(inputs[1],&script);
		
	}
	else if(strcmp(inputs[0],"d\n")==0)
	{
		printf("Enter filename:");
		fgets(inputs[1],99,stdin);
		remove_newlines(2,inputs);
		removefromrepo(inputs[1],&script);
	}
	else if(strcmp(inputs[0],"a\n")==0)
	{
		printf("Enter filename (fullpath including .sh):");
		fgets(inputs[1],99,stdin);
		printf("Enter no_args:");
		fgets(inputs[2],99,stdin);
		printf("Enter description (encapsulate with quotes):");
		fgets(inputs[3],99,stdin);

		remove_newlines(4,inputs);
		extract_filename(inputs[1],&script);
		script.no_args = atoi(inputs[2]);
		strcpy(script.description,inputs[3]);
	//	remove_newlines(3,inputs);
	//	printf("file to :%s\n",script.name);
		copytorepo(inputs[1],&script);
		log_script(&script);
	}	
}
