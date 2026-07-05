CC = gcc
CFLAGS = -Wall

all: writer_flock writer_fcntl

writer_flock: writer_flock.c
	$(CC) $(CFLAGS) -o writer_flock writer_flock.c

writer_fcntl: writer_fcntl.c
	$(CC) $(CFLAGS) -o writer_fcntl writer_fcntl.c

test_flock: writer_flock
	./writer_flock "test message flock"

test_fcntl: writer_fcntl
	./writer_fcntl "test message fcntl"

clean:
	rm -f writer_flock writer_fcntl system.log
