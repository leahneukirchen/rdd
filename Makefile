CFLAGS=-O3 -Wall
all: rdd

rdd: sosemanuk.o rdd.o

rdd.c: sosemanuk.h

clean:
	rm -f rdd rdd.o sosemanuk.o
