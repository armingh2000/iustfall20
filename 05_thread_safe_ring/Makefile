CC = gcc
CFLAGS = -Wall -O3 -lpthread -g
OBJECTS = producer_consumer.o not_safe_producer_consumer.o

default: $(OBJECTS)

clean:
	rm *.o

%.o: %.c
	$(CC) -o$@ $< $(CFLAGS)

.PHONY: clean
