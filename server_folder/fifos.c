#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fun.h"

int fifo_files_size = 0;
int fifo_sockets_size = 0;
int fifo_copy_files_size = 0;
int fifo_blocks_size = 0;

void push_files(char *str)
{
    if (fifo_files_size == 0)        // When we push for fisrt time we make an array from strings with one position and we put the element
    {
        fifo_files_size++;
        fifo_files = (char **)malloc(sizeof(char *) * fifo_files_size);
        fifo_files[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_files[0], str);
        new_fifo_files = (char **)malloc(sizeof(char *) * fifo_files_size);     // This array is useful for pops functions
        new_fifo_files[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_files[0], str);
    }
    else
    {                              // When we push for second,third time ... we increase the size of array and we put the element
        fifo_files_size++;
        fifo_files = realloc(fifo_files, sizeof(char *) * fifo_files_size);
        fifo_files[fifo_files_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_files[fifo_files_size - 1], str);
        new_fifo_files = realloc(new_fifo_files, sizeof(char *) * fifo_files_size);
        new_fifo_files[fifo_files_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_files[fifo_files_size - 1], str);
    }
}

char *pop_files()
{
    char *str = fifo_files[0];              // Every time we pop the first element from the array
    fifo_files = realloc(fifo_files, sizeof(char *) * fifo_files_size);
    for(int i = 0 ; i < fifo_files_size ; i++){        // We copy all elements from fifo to a new fifo
        new_fifo_files[i] = malloc((strlen(fifo_files[i])+1)*sizeof(char));
        strcpy(new_fifo_files[i],fifo_files[i]);
    }
    fifo_files_size--;
    fifo_files = realloc(fifo_files, sizeof(char *) * fifo_files_size);    //We decrease the size of fifo
    for (int i = 0; i < fifo_files_size; i++)           // We pop the first element from array
    {
        fifo_files[i] = malloc(strlen(fifo_files[i]+1)*sizeof(char));
        fifo_files[i] = new_fifo_files[i + 1];
    }
    return str;
}

void push_sockets(int client_socket)
{
    if (fifo_sockets_size == 0)
    {
        fifo_sockets_size++;
        fifo_sockets = (int *)malloc(sizeof(int) * fifo_sockets_size);
        fifo_sockets[0] = client_socket;
        new_fifo_sockets = (int *)malloc(sizeof(int) * fifo_sockets_size);
        new_fifo_sockets[0] = client_socket;
    }
    else
    {
        fifo_sockets_size++;
        fifo_sockets = realloc(fifo_sockets, sizeof(int) * fifo_sockets_size);
        fifo_sockets[fifo_sockets_size - 1] = client_socket;
        new_fifo_sockets = realloc(new_fifo_sockets, sizeof(int) * fifo_sockets_size);
        new_fifo_sockets[fifo_sockets_size - 1] = client_socket;
    }
}

int pop_sockets()
{
    int str = fifo_sockets[0];
    fifo_sockets = realloc(fifo_sockets, sizeof(int) * fifo_sockets_size);
    for(int i = 0 ; i < fifo_files_size ; i++) {
        new_fifo_sockets[i] = fifo_sockets[i];
    }
    fifo_sockets_size--;
    fifo_sockets = realloc(fifo_sockets, sizeof(int) * fifo_sockets_size);
    for (int i = 0; i < fifo_sockets_size; i++)
    {
        fifo_sockets[i] = new_fifo_sockets[i + 1];
    }
    return str;
}

void push_copy_files(char *str)
{
    if (fifo_copy_files_size == 0)
    {
        fifo_copy_files_size++;
        fifo_copy_files = (char **)malloc(sizeof(char *) * fifo_copy_files_size);
        fifo_copy_files[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_copy_files[0], str);
        new_fifo_copy_files = (char **)malloc(sizeof(char *) * fifo_copy_files_size);
        new_fifo_copy_files[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_copy_files[0], str);
    }
    else
    {
        fifo_copy_files_size++;
        fifo_copy_files = realloc(fifo_copy_files, sizeof(char *) * fifo_copy_files_size);
        fifo_copy_files[fifo_copy_files_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_copy_files[fifo_copy_files_size - 1], str);
        new_fifo_copy_files = realloc(new_fifo_copy_files, sizeof(char *) * fifo_copy_files_size);
        new_fifo_copy_files[fifo_copy_files_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_copy_files[fifo_copy_files_size - 1], str);
    }
}

void push_block(char *str)
{
    if (fifo_blocks_size == 0)
    {
        fifo_blocks_size++;
        fifo_blocks = (char **)malloc(sizeof(char *) * fifo_blocks_size);
        fifo_blocks[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_blocks[0], str);
        new_fifo_blocks = (char **)malloc(sizeof(char *) * fifo_blocks_size);
        new_fifo_blocks[0] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_blocks[0], str);
    }
    else
    {
        fifo_blocks_size++;
        fifo_blocks = realloc(fifo_blocks, sizeof(char *) * fifo_blocks_size);
        fifo_blocks[fifo_blocks_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(fifo_blocks[fifo_blocks_size - 1], str);
        new_fifo_blocks = realloc(new_fifo_blocks, sizeof(char *) * fifo_blocks_size);
        new_fifo_blocks[fifo_blocks_size - 1] = malloc(sizeof(char) * strlen(str) + 1);
        strcpy(new_fifo_blocks[fifo_blocks_size- 1], str);
    }
}