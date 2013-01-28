SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -std=c99 -Wall

test: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c -o $*.o

.PHONY: all clean

all: test

clean:
	rm -f *.o test