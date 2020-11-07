CC = gcc
CFLAGS = -Wall -Werror -O3
OBJECTS = single_proc.o multi_proc.o

default: $(OBJECTS)

clean:
	rm *.o

%.o: %.c
	$(CC) -o$@ $< $(CFLAGS)

.PHONY: clean
