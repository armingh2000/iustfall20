CC = gcc
CFLAGS = -Wall -O3 -lpthread -g
OBJECTS = producer_consumer.o not_safe_producer_consumer.o \
	  critical_region_example.o mutex_example.o

default: $(OBJECTS)

clean:
	rm *.o

%.o: %.c
	$(CC) -o$@ $< $(CFLAGS)

.PHONY: clean
