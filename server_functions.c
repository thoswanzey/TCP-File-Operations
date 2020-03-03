#include "server.h"



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