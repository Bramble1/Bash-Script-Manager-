#include "communicate.h"
#include<string.h>

/*signals
 * ------
 *  signals to determine what command the server should execute,if a file exists etc.*/
#define sendpackage "1\0"
#define requestAccepted "2\0"
#define packageExists "3\0"
#define invalidPackage "4\0"
#define query "5\0"
#define upload_package "6\0"
#define delete_package "7\0"
/*break_string() , revall() , sendall() , check_command()*/

/*Break down a string command sent from the client to determine what  command to execute and any additional args.
 * -------------------------------------------------------------------------------------------------------------
 *  1-iterate through command, and place within multi array.
 *  2- if a " " is encountered then end first array and place the rest of the text after " " into a new array.
 *  3- repeat until there are is no " "
 *  4- return the multi array ,containing the command and arguments in seperate arrays.
 * */
char (*break_string(char brkn_command[3][20],char *command))[20]
{
	int pos , iter ,x;
	pos = iter = x = 0;
	/*1. 3.*/
	for(int i=0; i<strlen(command);i++)
	{   /*2.*/
		if(command[i] == ' ')
		{
			brkn_command[pos][x] = '\0';
			pos++;
			x = 0;
			continue;
		}/*1.*/
		else if(command[i] != ' ')
		{
			brkn_command[pos][x] = command[i];
		}
		x++;
		iter = x;
	}
	brkn_command[pos][iter] = '\0';
/*4.*/	return brkn_command;	
}

/*Satantize the string and make sure any commands do not contain any special characters.
 * -------------------------------------------------------------------------------------
 *  1-iterate through string command
 *  2- if any of the characters is not an alphabetical character then error*/
void sanitize_string(char *string)
{
	/*1.*/
	for(int i=0;i<strlen(string);i++)
	{
		/*2.*/
		if(((string[i]>=0x41 && string[i]<=0x5a) || (string[i]>=0x61 && string[i]<=0x7a)))
		{
			//printf("%c\n",string[i]);
		} 
		else
		{
			perror("string contains non-alphabetical characters");
			exit(1);
		}
	}
}
