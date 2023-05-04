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

int queue_size = 0;
int block_size = 0;

pthread_mutex_t lock; // lock for push
pthread_cond_t c, w;  // signals for communication and worker thread

int main(int argc, char const *argv[])
{
    int port, thread_pool_size;
    int sock, newsock;
    socklen_t clientlen;
    struct sockaddr_in server = {0};
    struct sockaddr_in client = {0};
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct sockaddr *clientptr = (struct sockaddr *)&client;
    struct hostent *rem;

    if (argc != 5)
    {
        printf("Try again wrong arguments!\n");
        exit(EXIT_FAILURE);
    }
    printf("Server's parameters are:\n");
    port = atoi(argv[1]);
    printf("port: %d\n", port);
    thread_pool_size = atoi(argv[2]);
    printf("thread_pool_size: %d\n", thread_pool_size);
    queue_size = atoi(argv[3]);
    printf("queue_size: %d\n", queue_size);
    block_size = atoi(argv[4]);
    printf("block_size: %d\n", block_size);

    signal(SIGCHLD, sigchld_handler); // reap dead children asynchronously

    pthread_t thread_pool[thread_pool_size];
    for (int i = 0; i < thread_pool_size; i++)
    {
        pthread_create(&thread_pool[i], NULL, worker_thread, NULL); // first create thread pools
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&c, NULL) != 0)
    {
        perror("pthread_cond_init");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&w, NULL) != 0)
    {
        perror("pthread_cond_init");
        exit(EXIT_FAILURE);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(sock, serverptr, sizeof(server)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("Server wa successfully initialized...\n");

    if (listen(sock, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listening for connections to port %d\n", port);

    while (1)
    {
        clientlen = sizeof(client);
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if ((rem = gethostbyaddr((char *)&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL)
        {
            perror("gethostbyaddr");
            exit(EXIT_FAILURE);
        }
        printf("Accepted connection from %s\n", rem->h_name);

        pthread_t t;
        int *pclient = malloc(sizeof(int)); // create a pointer for newsock for parallels clients
        *pclient = newsock;
        pthread_create(&t, NULL, communication_thread, pclient);
    }
    free(fifo_files);
    free(new_fifo_files);
    fifo_files_size = 0;
    free(fifo_sockets);
    free(new_fifo_sockets);
    fifo_sockets_size = 0;
    close(newsock);
}

/* Wait for all dead child processes */
void sigchld_handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}