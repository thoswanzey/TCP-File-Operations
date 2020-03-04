// The echo client client.c
#include "client.h"


// Define GLOBAL variables
struct sockaddr_in  server_addr; 
int sock, r;
char line[MAX];
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";


int main(int argc, char *argv[ ])
{
  int n;
  char ans[MAX];

  client_init();

  printf("********  processing loop  *********\n");
  while (1){
    printf(BOLD CYN"input a line : "RESET);
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
      exit(0);

    if(!local_command_handler()){
      // Send ENTIRE line to server
      n = write(sock, line, MAX);
      printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

      // Read a line from sock and show it
      n = read(sock, ans, MAX);
      printf("client: read  n=%d bytes; echo=("RED"%s"RESET")\n", n, ans);
    }
  }
}


