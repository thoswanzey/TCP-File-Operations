#include "server.h"

#define  MAX 256

extern int csock;


int command_cd(int nargs, char * args[])
{
    int fail = -1;
    if(nargs == 2)
    {
        fail = chdir(args[1]);
    }
    else if(nargs == 1)
    {
        fail = chdir(getenv("HOME"));
    }
    
    return fail;
}


int command_rm(int nargs, char * args[]){
    char message[MAX];
    int fail = -1;
    if(nargs == 2){
        fail = unlink(args[1]);
    }
    if(fail)
    {
        strcpy(message, "Invalid Command!\n");
    }
    else
    {
        strcpy(message, args[1]);
        strcat(message, " was removed\n");
    }
    
    return write(csock, message, sizeof(message));
}


int command_rmdir(int nargs, char * args[]){
    char message[MAX];
    int fail = -1;
    if(nargs == 2){
        fail = rmdir(args[1]);
    }

    if(fail)
        strcpy(message, "Invalid Command!\n");
    else{
        strcpy(message, "The directory ");
        strcat(message, args[1]);
        strcat(message, " was successfully removed\n");
    }

    return write(csock, message, sizeof(message));
}


int command_mkdir(int nargs, char * args[]){
    char message[MAX];
    int fail = -1;
    if(nargs == 2){
        fail = mkdir(args[1], 0766);
    }

    if(fail)
        strcpy(message, "Invalid Command!\n");
    else{
        strcpy(message, "The directory ");
        strcat(message, args[1]);
        strcat(message, " was successfully created\n");
    }

    return write(csock, message, sizeof(message));
}