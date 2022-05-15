circular.o:
	c99 -Wall -o circular.o circular.c -I. -c

main.o:
	c99 -Wall -o main.o -c main.c -I. -c

all: circular.o main.o
	c99 -Wall -o main main.o circular.o

clean:
	rm main.o circular.o main
