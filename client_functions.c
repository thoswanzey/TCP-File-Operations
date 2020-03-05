#include "client.h"

extern char line[MAX];
extern int sock, r;
extern struct sockaddr_in  server_addr; 
extern char *t1;
extern char *t2;

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
    char buf[MAX];

    int nargs, i = 1;
    char *token, *args[32];
    char command[MAX];
    strcpy(command, line);
    args[0] = strtok(command, " ");
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
    else if(!strcmp(args[0], "lcat")){
        command_lcat(nargs, args);
    }
    else if(!strcmp(args[0], "lls")){
        command_lls(nargs, args);
    }
    else if(!strcmp(args[0], "get")) {
        bzero(buf, MAX);
        get_file(args[1]);
    }
    else if(!strcmp(args[0], "put")) {
        bzero(buf, MAX);
        strcat(buf, "put ");
        strcat(buf, args[1]);
        write(sock, buf, MAX); // send file request
        send_file(args[1]);
    }
    else if(!strcmp(args[0], "ls"))
    {
        command_ls(nargs, args[1]);
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


void command_lcat(int nargs, char * args[]){
  int fd, n;
  char buf[BLKSIZE];
  fd = -1;

    if (nargs == 2){
        fd = open(args[1], O_RDONLY);
    }

    if (fd >= 0){
        while (n = read(fd, buf, BLKSIZE) ){
            write(1, buf, n);
        }
    }
    else
        printf(RED"Invalid Command!\n"RESET);
  
}

int command_lls(int nargs, char * args[])
{
    struct stat mystat, *sp = &mystat;
    int r;
    char *filename, path[1024], cwd[256];
    filename = "./"; // default to CWD
    if(nargs > 2){
        printf(RED"Invalid Command!\n"RESET);
        return -1;
    }
    else if (nargs == 2)
        filename = args[1]; // if specified a filename
    if (r = lstat(filename, sp) < 0){
        printf("no such file %s\n", filename);
        return -1;
    }
    strcpy(path, filename);
    if (path[0] != '/'){ // filename is relative : get CWD path
        getcwd(cwd, 256);
        strcpy(path, cwd); strcat(path, "/"); strcat(path,filename);
    }
    if (S_ISDIR(sp->st_mode))
        ls_dir(path);
    else
        ls_file(path);
}


void ls_dir(char *dname)
{
    struct dirent *ep;
    DIR *dp = opendir(dname);
    char temp[1000];
    while (ep = readdir(dp)){
        strcpy(temp, dname);
        strcat(temp, "/");
        strcat(temp, ep->d_name);
        ls_file(temp);
    }
}

int ls_file(char *fname)
{
    struct stat fstat, *sp;
    int r, i;
    char ftime[64];
    sp = &fstat;
    if ( (r = lstat(fname, &fstat)) < 0){
        printf("\ncan’t stat %s\n", fname);
        return -1;
    }

    if ((sp->st_mode & 0xF000) == 0x8000) // if (S_ISREG())
        printf("%c",'-');
    if ((sp->st_mode & 0xF000) == 0x4000) // if (S_ISDIR())
        printf("%c",'d');
    if ((sp->st_mode & 0xF000) == 0xA000) // if (S_ISLNK())
    printf("%c",'l');
    for (i=8; i >= 0; i--){
        if (sp->st_mode & (1 << i)) // print r|w|x
            printf("%c", t1[i]);
        else
            printf("%c", t2[i]); // or print -
    }
    printf("%4lud ",sp->st_nlink); // link count
    printf("%4d ",sp->st_gid); // gid
    printf("%4d ",sp->st_uid); // uid
    printf("%8ld ",sp->st_size); // file size
    // print time
    strcpy(ftime, ctime(&sp->st_ctime)); // print time in calendar form
    ftime[strlen(ftime)-1] = 0; // kill \n at end
    printf("%s ",ftime);
    // print name
    printf("%s", basename(fname)); // print file basename
    // print -> linkname if symbolic file
    if ((sp->st_mode & 0xF000)== 0xA000){
        char linkname[512];
        readlink(fname, linkname, 512);
        // use readlink() to read linkname
        printf(" -> %s", linkname); // print linked name
    }
    printf("\n");
}

void command_ls(int nargs, char *dir)
{
    char buf[MAX], filebuf[MAX];
    bzero(buf, MAX);
    bzero(filebuf, MAX);
    
    if(nargs > 1) // if directory or file arg
    {
        strcat(filebuf, "ls ");
        strcat(filebuf, dir);
        write(sock, filebuf, MAX); // send file request
    }
    else
    {
        write(sock, "ls", sizeof("ls"));
    }
    
    int n;
    
    while(strcmp(buf, "\r\r")) // read lines until EOF
    {
        n = read(sock, buf, MAX); // get file size
        printf("%s\n", buf);
    }
}

int get_file(char *filename) {
   int count = 0;
   FILE *recv_file;
   
   char buf[MAX], filebuf[MAX];
   bzero(buf, MAX);
   
   int n, file_size = 0;

   strcat(filebuf, "get ");
   strcat(filebuf, filename);
   write(sock, filebuf, MAX); // send file request
   read(sock, buf, MAX); // get file size

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
      n = read(sock, buf, MAX);
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
      sent_bytes = write(sock, "BAD", MAX);
      return 0;
   }
   printf("File size: %ld\n", file_stat.st_size);

   char size[30];
   sprintf(size, "%ld", file_stat.st_size);

   sent_bytes = write(sock, size, MAX);

   int remain_data = file_stat.st_size;

   while(sent_bytes = read(fd, buf, MAX)) {
      write(sock, buf, sent_bytes);
      remain_data -= sent_bytes;
      printf("Server sent %d bytes from file, remaining data: %d\n", sent_bytes, remain_data);
   }

   close(fd);
}