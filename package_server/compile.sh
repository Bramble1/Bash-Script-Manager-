#!/bin/bash

gcc -o server stream_server.c network_functions/files.c network_functions/communicate.c network_functions/server_setup.c
