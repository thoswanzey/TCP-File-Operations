#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <netdb.h>

int command_cd(int nargs, char * args[]);
int command_rm(int nargs, char * args[]);
int command_rmdir(int nargs, char * args[]);
int command_mkdir(int nargs, char * args[]);



#endif