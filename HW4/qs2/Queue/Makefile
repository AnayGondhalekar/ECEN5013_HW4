CC=gcc

CFLAGS= -O0 -g -Wall -Werror

CFILES= queue.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all: queue

clean:
	-rm -f *.o *.d
	-rm -f queue

queue: queue.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ -lrt

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
