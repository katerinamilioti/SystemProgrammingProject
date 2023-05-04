OBJS	= client.o
SOURCE	= client.c
OUT  	= myclient
CC	= gcc
FLAGS   = -g -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

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
