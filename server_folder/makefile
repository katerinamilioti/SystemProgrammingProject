#
# In order to execute this "Makefile" just type "make"
#

OBJS	= server.o fifos.o comm.o worker.o
SOURCE	= server.c fifos.c comm.c worker.c
HEADER  = fun.h
OUT  	= myserver
CC	= gcc
FLAGS   = -g 	-c 	-pedantic 	-ansi	-Wall -pthread
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ -pthread

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

LinkedList.o: LinkedList.c
	$(CC) $(FLAGS) LinkedList.c

OrderedLinkedList.o: OrderedLinkedList.c
	$(CC) $(FLAGS) OrderedLinkedList.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)