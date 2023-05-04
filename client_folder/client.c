#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void makeDir(char *path,int lengt,int);

int main(int argc, char const* argv[])
{
    char *server_ip  , *directory;
    int server_port;
    int sock;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct in_addr ip;
    struct hostent *hp;
    if(argc != 4){
        printf("Try again wrong arguments!\n");
        exit(1);
    }
    printf("Client's parameters are:\n");
    server_ip = (char *)malloc((strlen(argv[1]) +1)*sizeof(char));
    if(server_ip == NULL){
        printf("Memory for server_ip not allocated.\n");
        exit(2);
    }
    strcpy(server_ip, argv[1]);
    printf("serverIP: %s\n",server_ip);
    server_port = atoi(argv[2]);
    printf("port: %d\n",server_port);
    directory = (char *)malloc((strlen(argv[3])+1)*sizeof(char));
    if(directory == NULL){
        printf("Memory for directory not allocated.\n");
        exit(3);
    }
    strcpy(directory,argv[3]);
    printf("directory: %s\n",directory);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);

    if (inet_pton(AF_INET,server_ip,  &server.sin_addr) <= 0){          // converts the character string src into a networkaddress structure in the af address family, then copies thenetwork address structure to dst
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, serverptr, sizeof(server)) < 0){
           perror("connect");
       exit(EXIT_FAILURE);
    }
    printf("Connecting to %s port %d\n", server_ip, server_port);
    write(sock, directory, strlen(directory)+1);
    int block_size;
    read(sock,&block_size,sizeof(int));            // read the size of every block
    block_size = ntohl(block_size);             // converts the unsigned integer netlong from network byte order to host byte order
    int files;
    read(sock, &files, sizeof(int));        // read how many files there are into folder
    files = ntohl(files);

    int length_name;
    char *buff;
    for(int i = 0 ; i < files ; i++){
        read(sock,&length_name,sizeof(int));        // read the legth of path
        length_name = ntohl(length_name);
        buff = malloc((length_name)*sizeof(char));
        read(sock,buff,length_name);                // read the name of path
        buff[length_name] = '\0';
        printf("Received: %s\n",buff);
        makeDir(buff,block_size,sock);
        free(buff);
    }
    close(sock);
    free(server_ip);
    free(directory);
}

void makeDir(char *buff,int block_size,int sock)
{
    char *token;
    char *blocks;
    char paths[1000][1000];
    int max = 0;
    token = strtok(buff, "/");
    while (token != NULL)
    {
        strcpy(paths[max], token);
        max++;
        token = strtok(NULL, "/");           // find all tokens from a path such as Server/folder1/file1 : Server, folder , file1
    }
    mkdir(paths[0],0700);                   // create the request folder
    for(int k = 1 ; k < max ; k++){
        strcat(paths[0],"/");
        strcat(paths[0],paths[k]);
        if(k!=max-1){                       
            mkdir(paths[0],0700);       // we create every folder that exists
        }
        else{                                   // we are in the last token so we have the path of file
            if(access(paths[0], F_OK) == 0){    // if file already exists we remove it 
                remove(paths[0]);
            }
            creat(paths[0],0700);
            int fd = open(paths[0], O_WRONLY,0700);
            int block;
            read(sock,&block,sizeof(int));       // we read how many blocks exists
            block = ntohl(block);
            for(int j = 0 ; j < block; j++){
                blocks = malloc((block_size+1)*sizeof(char));
                read(sock,blocks,block_size+1);
                write(fd,blocks,strlen(blocks));
                free(blocks);
            }
            close(fd);
        }
    }
}
