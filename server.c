// This is the echo SERVER server.c
#include "server.h"

#define  MAX 256
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 1234
// Define variables:
struct sockaddr_in  server_addr, client_addr;
int mysock, csock;
int r, len, n;

// Server initialization code:
int server_init()
{
   printf("==================== server init ======================\n");   
   // get DOT name and IP address of this host
   printf("1 : create a TCP STREAM socket\n");
   mysock = socket(AF_INET, SOCK_STREAM, 0);
   
   printf("2 : fill server_addr with host IP and PORT# info\n");
   // initialize the server_addr structure
   server_addr.sin_family = AF_INET;                  // for TCP/IP
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
   server_addr.sin_port = htons(SERVER_PORT);         // port number

   printf("3 : bind socket to server address\n");
   // bind syscall: bind the socket to server_addr info
   r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
   if (r < 0){
       printf("bind failed\n");
       exit(3);
   }
   printf("host = %s port = %d\n", SERVER_HOST, SERVER_PORT);
   
   printf("4 : server is listening ....\n");
   listen(mysock, 5); // listen queue length = 5
   printf("===================== init done =======================\n");
}

void command_select(char *cmd)
{
   char * args[32];
   int nargs;
   char * token;
   int i = 1;
   args[0] = strtok(cmd, " ");
   
   while(token = strtok(NULL, " ")){
      args[i] = token;
      i++;
   }
   nargs = i;

   if(!strcmp(args[0], "pwd")) // return current working directory
   {
      char cwd[MAX];
      getcwd(cwd, sizeof(cwd));
      n = write(csock, cwd, MAX);
   }
   else if(!strcmp(args[0], "cd")){
      if(command_cd(nargs, args))
         n = write (csock, "Invalid Command!\n", sizeof("Invalid Command!\n"));
      else
         n = write (csock, "Directory successfully changed!\n", sizeof("Directory successfully changed!\n"));  
   }
   else{
      n = write (csock, "Invalid Command!\n", sizeof("Invalid Command!\n"));
   }
}

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

