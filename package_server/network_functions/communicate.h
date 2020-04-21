#include <stdio.h>
#include <stdlib.h>

#define sendpackage "1\0"
#define requestAccepted "2\0"
#define packageExists "3\0"
#define invalidPackage "4\0"
#define query "5\0"
#define upload_package "6\0"
#define delete_package "7\0"
char (*break_string(char brkn_command[3][20],char *command))[20];
void sanitize_string(char *string);
