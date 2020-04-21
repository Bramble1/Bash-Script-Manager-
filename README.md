# Bash-Script-Manager-
(In progress, not complete)

C program to manage bash scripts in a central repository. Useful for anyone who writes a lot of bash scripts such as system adminitrators and sometimes forget what a certain script does. With this program you can easily get access to ur scripts, query a script and view a user defined description of the script so you can remember what the script does, how many arguments it requires etc.

As well as be able to execute and schedule any scripts in ur repo. WIthout having to provide a full path or cd and find the correct directory with ur script. As well as backup or download scripts to and from a server.


# Functions

 1. ./manage_repo - delete,add,query scripts (view description about script and how many arguments etc)
 
 2. ./execute - execute scripts or schedule scripts 
 
 3. ./upload_download - upload and download scripts to the server 
 
 
 # Design
 
 Modular design. So the programs can be used individually, like command line programs, or together within ./wrapper. This allows to use each function/feature individually or for the user to create their own wrapper to manage the executables.
