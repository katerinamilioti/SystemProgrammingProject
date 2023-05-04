extern int fifo_files_size;
extern char **fifo_files;
extern char**new_fifo_files;

extern int fifo_sockets_size;
extern int *fifo_sockets;
extern int *new_fifo_sockets;

extern int fifo_copy_files_size;
extern char **fifo_copy_files;
extern char **new_fifo_copy_files;

extern int fifo_blocks_size;
extern char **fifo_blocks;
extern char **new_fifo_blocks;

extern int queue_size ;
extern int block_size;

extern pthread_mutex_t lock;
extern pthread_cond_t c,w;

extern int queue_size ;
extern int block_size;


void sigchld_handler(int );
void *communication_thread(void * );
void find_files(char * , int );
void count_files(char * , int );
void write_paths(int );
void *worker_thread(void * );
void push_files(char * );
char *pop_files(void );
void push_sockets(int );
int pop_sockets(void );
void push_copy_files(char * );
void push_block(char * );

