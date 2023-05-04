#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"fun.h"

int fifo_files_size;
char **fifo_files;
char **new_fifo_files;

int fifo_sockets_size;
int *fifo_sockets;
int *new_fifo_sockets;

int fifo_copy_files_size;
char **fifo_copy_files;
char **new_fifo_copy_files;

int fifo_blocks_size;
char **fifo_blocks;
char **new_fifo_blocks;

int queue_size;
int block_size;

pthread_mutex_t lock;
pthread_cond_t c, w;

void *communication_thread(void *p_client_socket)
{
    //sleep(10);                                 // sleep is for checking parallels clients
    int n = 0;
    char *buff;
    char b[1];
    while (1)
    {
        if((read(*((int *)p_client_socket), b, sizeof(b))) == -1){  // read the folder request from client and put it dynamically into an array
            perror("read");
            exit(EXIT_FAILURE);
        }    
        n++;
        if (n == 1)
        {
            buff = malloc(n * sizeof(char));
            if (buff == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            buff[0] = b[0];
        }
        else
        {
            buff = realloc(buff, n * sizeof(char));
            if (buff == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            buff[n - 1] = b[0];
        }
        if (b[0] == '\0')
        {
            buff = realloc(buff, n);
            if (buff == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            buff[n - 1] = '\0';
            break;
        }
    }
    printf("[Thread: %ld]: About to scan directory %s\n", pthread_self(), buff);
    find_files(buff, *((int *)p_client_socket));
    write_paths(*((int *)p_client_socket));
    free(buff);
    memset(b, 0, sizeof(b));
    pthread_exit(NULL);
}
/* find all files inside the request folder recursively */
void find_files(char *dirname,int p_client_socket)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    entry = readdir(dir);
    while (entry != NULL)
    {
        if ((entry->d_type == 4) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)    // 4 is for folder and we skip .(current directory) and ..(parent directory of the directory)
        {
            char path[1024];
            memset(path, 0, sizeof(path));
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entry->d_name);
            find_files(path,p_client_socket);
        }
        if (entry->d_type == 8)            // 8 is for file 
        {
            char file_path[1024];
            memset(file_path, 0, sizeof(file_path));
            strcat(file_path, dirname);
            strcat(file_path, "/");
            strcat(file_path, entry->d_name);
            push_copy_files(file_path);       //we put the path of the file into a fifo
        }
        entry = readdir(dir);
    }
    closedir(dir);
}

void write_paths(int p_client_socket)
{
    int net_block_size = htonl(block_size);                           //converts the unsigned integer hostlong from host byte order to network byte order
    if((write(p_client_socket, &net_block_size, sizeof(int))) == -1){   // write the size of block to socket 
        perror("write");
        exit(EXIT_FAILURE);
    }
    int net_fifo_copy_files_size = htonl(fifo_copy_files_size);
    if((write(p_client_socket, &net_fifo_copy_files_size, sizeof(int))) == -1){     // write how many files there are for the clietn to know how much to read
        perror("write");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&lock);                                         // when is going to be pushed a file into fifo we lock for not being many pushes from other thread simultaneously
    for(int i = 0 ;i < fifo_copy_files_size; i++){
        printf("[Thread: %ld]: Adding file %s to the queue...\n", pthread_self(),fifo_copy_files[i]);
        push_files(fifo_copy_files[i]);                               
        push_sockets(p_client_socket);
        pthread_cond_signal(&w);                                    // signal for worker that fifo is not empty
        if (fifo_files_size == queue_size)
        {
            pthread_cond_wait(&c, &lock);                         // signal for communication thread to wait until worker pop from fifo
        }
    }
    pthread_mutex_unlock(&lock);                                // when we did push unlock mutex for other thread
    free(fifo_copy_files);
    free(new_fifo_copy_files);
    fifo_copy_files_size = 0;
}