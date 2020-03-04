// This is the echo SERVER server.c
#include "server.h"

// Define variables:
struct sockaddr_in  server_addr, client_addr;
int mysock, csock;
int r, len, n;


int main(int argc, char *argv[])
{
   char line[MAX], echo[MAX];

   server_init(); 
   
   while(1){    // Try to accept a client request
     printf("server: accepting new connection ....\n"); 

     // Try to accept a client connection as descriptor csock
     len = sizeof(client_addr);
     csock = accept(mysock, (struct sockaddr *)&client_addr, &len);
     if (csock < 0){
        printf("server: accept error\n");
        exit(1);
     }
     printf("server: accepted a client connection from\n");
     printf("-----------------------------------------------\n");
     printf("Client: IP=%s  port=%d\n", "127.0.0.1",
                            ntohs(client_addr.sin_port));
     printf("-----------------------------------------------\n");

     // Processing loop: csock <-- data --> client
     while(1){
       n = read(csock, line, MAX);
       if (n==0){
            printf("server: client died, server loops\n");
            int true = 1;
            setsockopt(csock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)); // tell kernel it's okay to rebind socket
            close(csock); // close socket
           break;
      }

      command_select(line);

      printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, line);
      printf("server: ready for next request\n");
    }
 }
}

