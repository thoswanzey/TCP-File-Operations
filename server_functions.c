#include "server.h"

extern struct sockaddr_in  server_addr;
extern int mysock, csock;
extern int n, r;

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
    else if(!strcmp(args[0], "cd"))
    {
        if(command_cd(nargs, args))
            n = write (csock, "Invalid Command!\n", sizeof("Invalid Command!\n"));
        else
            n = write (csock, "Directory successfully changed!\n", sizeof("Directory successfully changed!\n"));  
    }
    else if(!strcmp(args[0], "get")) {
        send_file(args[1]);
    }
    if(!strcmp(args[0], "put")) {
        get_file(args[1]);
    }
    else
    {
        n = write (csock, "Invalid Command!\n", sizeof("Invalid Command!\n"));
    }
}

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

int get_file(char *filename) {
   int count = 0;
   FILE *recv_file;
   
   char buf[MAX];
   bzero(buf, MAX);
   
   int n, file_size = 0;

   read(csock, buf, MAX); // get file size

   if(strcmp(buf, "BAD") == 0 || buf == NULL) {
      printf("GET: Error receiving filesize from server!\n");
      return 0;
   }

   file_size = atoi(buf);
   printf("FILE SIZE: %d\n", file_size);
   recv_file = fopen(filename, "w");

   if (recv_file == NULL) {
      printf("GET: Failed to acquire file!\n");
      return 0;
   }

   while (count < file_size) {
      n = read(csock, buf, MAX);
      count += n;
      fwrite(buf, sizeof(char), n, recv_file);
   }
   
   fclose(recv_file);
}  

int send_file(char *filename) {
   char buf[MAX];
   bzero(buf, MAX);

   struct stat file_stat;
   int filesize, sent_bytes;
   int fd = open(filename, O_RDONLY);

   if(fstat(fd, &file_stat) < 0) {
      sent_bytes = write(csock, "BAD", MAX);
      return 0;
   }
   printf("File size: %ld\n", file_stat.st_size);

   char size[30];
   sprintf(size, "%ld", file_stat.st_size);

   sent_bytes = write(csock, size, MAX);

   int remain_data = file_stat.st_size;

   while(sent_bytes = read(fd, buf, MAX)) {
      write(csock, buf, sent_bytes);
      remain_data -= sent_bytes;
      printf("Server sent %d bytes from file, remaining data: %d\n", sent_bytes, remain_data);
   }

   close(fd);
}