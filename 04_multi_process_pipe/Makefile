CC = gcc
CFLAGS = -Wall -O3 -lpthread
OBJECTS = single_proc.o multi_proc.o multi_thread.o

default: $(OBJECTS)

clean:
	rm *.o

%.o: %.c
	$(CC) -o$@ $< $(CFLAGS)

.PHONY: clean
