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
#include "fun.h"


void *worker_thread(void *arg){
    while (1)
    {
        while(fifo_files_size == 0)
        {
            pthread_cond_wait(&w, &lock);           // signal for worker to wait while fifo is empty
        }
        char *path = pop_files();
        int sock = pop_sockets();
        pthread_cond_signal(&c);                    // signal to communication that fifo is empty
        printf("[Thread: %ld]: Received task: <%s, %d>\n", pthread_self(),path,sock);
        path[strlen(path)] = '\0';
        int path_length = htonl(strlen(path)+1);
        if((write(sock, &path_length, sizeof(int))) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        if((write(sock,path,strlen(path)+1)) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        int filedes;
        char buffer[block_size];
        memset(buffer, 0, strlen(buffer)+1);
        int length;
        char *file_name = path;
        printf("[Thread: %ld]: About to read file %s\n", pthread_self(),path);
        if ((filedes = open(file_name, O_RDONLY, 0777)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        length = lseek(filedes, 0, SEEK_END) + 1;       //find the length of file
        lseek(filedes, 0, SEEK_SET);                    //moves file pointer position to the beginning of the file
        int curr_size = 0;
        while (1)
        {
            curr_size = curr_size + block_size;
            if (read(filedes, buffer, sizeof(buffer)) == -1){         // read file block by block
                perror("read");
                exit(EXIT_FAILURE);
            }
            buffer[block_size] = '\0';
            push_block(buffer);                         // make a fifo with every block
            memset(buffer, 0, strlen(buffer)+1);
            if (curr_size >= length)                  // this means that we have read all blocks in file
            {
                break;
            }
        }
        int net_fifo_blocks_size = htonl(fifo_blocks_size);     // write how many blocks we have for client to know how many reads to do
        if((write(sock, &net_fifo_blocks_size, sizeof(int))) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        for(int j = 0  ; j < fifo_blocks_size ; j++){
            if((write(sock, fifo_blocks[j],block_size+1)) == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        fifo_blocks_size = 0;
        free(fifo_blocks);
        free(new_fifo_blocks);
        close(filedes);
    }
}