#ifndef SERVER_H
#define SERVER_H

#define  MAX 256
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 1235

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
#include <time.h>
#include <libgen.h>

void command_select(char *cmd);
int server_init();
int ls_file(char *fname);
void ls_dir(char *dname);
int command_ls(int nargs, char * args[]);
int command_cd(int nargs, char * args[]);
int command_rm(int nargs, char * args[]);
int command_rmdir(int nargs, char * args[]);
int command_mkdir(int nargs, char * args[]);
int get_file(char *filename);
int send_file(char *filename);


#endif

#ifndef COLOR_H
#define COLOR_H

    /* FANCY */
    #define RED   "\x1B[31m"
    #define GRN   "\x1B[32m"
    #define YEL   "\x1B[33m"
    #define BLU   "\x1B[34m"
    #define MAG   "\x1B[35m"
    #define CYN   "\x1B[36m"
    #define WHT   "\x1B[37m"
    #define RESET "\x1B[0m"
    #define BOLD  "\033[1m"

#endif // COLOR_H