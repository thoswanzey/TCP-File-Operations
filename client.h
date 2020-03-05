#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <libgen.h>

#define BLKSIZE 4096
#define MAX          256
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT  1235



int client_init(void);
int local_command_handler(void);
void command_lcd(int nargs, char * args[]);
void command_lrm(int nargs, char * args[]);
void command_lrmdir(int nargs, char * args[]);
void command_lmkdir(int nargs, char * args[]);
void command_lcat(int nargs, char * args[]);
int command_lls(int nargs, char * args[]);
void ls_dir(char *dname);
int ls_file(char *fname);
int get_file(char *filename);
int send_file(char *filename);
void command_ls(int nargs, char *dir);

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