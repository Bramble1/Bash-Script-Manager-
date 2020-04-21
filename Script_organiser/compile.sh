#!/bin/bash
echo "#define REPO_PATH \"$(pwd)/scripts_repo/\0\"" > execution_path.h

gcc -o manage_repo/manage_repo manage_repo/manage_repo.c common_headers/common_functions.c
gcc -o execute_scripts/execute execute_scripts/execute.c common_headers/common_functions.c
gcc -o execute_scripts/schedule execute_scripts/schedule_daemon.c
gcc -o upload_download_scripts/manage_server upload_download_scripts/manage_server.c upload_download_scripts/network_functions/files.c upload_download_scripts/network_functions/communicate.c upload_download_scripts/network_functions/client_setup.c common_headers/common_functions.c

cp manage_repo/manage_repo Wrapper/
cp execute_scripts/execute Wrapper/
cp execute_scripts/schedule Wrapper/
cp upload_download_scripts/manage_server Wrapper/

gcc -o Wrapper/wrapper Wrapper/wrapper.c common_headers/common_functions.c 
