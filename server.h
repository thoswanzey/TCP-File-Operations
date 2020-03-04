#ifndef SERVER_H
#define SERVER_H

#define  MAX 256
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 1234

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>

void command_select(char *cmd);
int server_init();
int command_cd(int nargs, char * args[]);
int send_file(char *filename);
int get_file(char *filename);



#endif