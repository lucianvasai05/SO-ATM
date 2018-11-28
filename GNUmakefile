CC=gcc
CFLAGS=-Wall -fPIC

all: build

build: tema1.o ListFunc.o HashFunc.o
	gcc -m32 -Wall $? -o tema1 -lhash -L.

tema1.o: tema1.c
	gcc -m32 -c tema1.c

ListFunc.o: ListFunc.c
	gcc -m32 -c ListFunc.c

HashFunc.o: HashFunc.c
	gcc -m32 -c HashFunc.c

clean:
	rm -f *.o
