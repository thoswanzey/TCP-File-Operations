#include "client.h"

extern char line[MAX];
extern int sock, r;
extern struct sockaddr_in  server_addr; 


// clinet initialization code
int client_init(void)
{
  printf("======= clinet init ==========\n");
  
  printf("1 : create a TCP socket\n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock<0){
     printf("socket call failed\n");
     exit(1);
  }

  printf("2 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htonl((127<<24) + 1);
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("3 : connecting to server ....\n");
  r = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(3);
  }
  printf("4 : connected OK to\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s PORT=%d\n", SERVER_HOST, SERVER_PORT);
  printf("---------------------------------------------------------\n");
  printf("========= init done ==========\n");
}



int local_command_handler(void){
  int nargs, i = 1;
  char *token, *args[32];
  args[0] = strtok(line, " ");
  while(token = strtok(NULL, " ")){
      args[i] = token;
      i++;
  }
   nargs = i;

  char cwd[MAX];
  getcwd(cwd, sizeof(cwd));

  if(!strcmp(args[0], "lpwd")){
    printf("%s\n", cwd);
  }
  else if(!strcmp(args[0], "quit")){
    printf("\nExiting...\n");
    exit(0);
  }
  else if(!strcmp(args[0], "lcd")){
    command_lcd(nargs, args);
  }
  else if(!strcmp(args[0], "lrm")){
    command_lrm(nargs, args);
  }
  else if(!strcmp(args[0], "lrmdir")){
    command_lrmdir(nargs, args);
  }
  else if(!strcmp(args[0], "lmkdir")){
    command_lmkdir(nargs, args);
  }
  //..
  else{
    return 0;  //Pass to server
  }

  return 1;    //local command, so no need to send to server

}


void command_lcd(int nargs, char * args[])
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
    
    if(fail)
      printf(RED"Invalid Command!\n"RESET);
    else
      printf("Successfuly changed directory!\n");
}


void command_lrm(int nargs, char * args[]){
    int fail = -1;
    if(nargs == 2){
        fail = unlink(args[1]);
    }
    if(fail)
        printf(RED"Invalid Command!\n"RESET);
    else
        printf(GRN "%s" RESET " was removed\n", args[1]);

}

void command_lrmdir(int nargs, char * args[]){
    int fail = -1;
    if(nargs == 2){
        fail = rmdir(args[1]);
    }

    if(fail)
        printf(RED"Invalid Command!\n"RESET);
    else
        printf("The directory " GRN "%s" RESET " was successfully removed\n", args[1]);
}

void command_lmkdir(int nargs, char * args[]){
    int fail = -1;
    if(nargs == 2){
        fail = mkdir(args[1], 0766);
    }

    if(fail)
        printf(RED"Invalid Command!\n"RESET);
    else
        printf("The directory " GRN "%s" RESET " was successfully created\n", args[1]);
}


