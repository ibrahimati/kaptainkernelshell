CC = -pedantic -Wall

all: kks

kks: main.c
	gcc $(CC) -o kks main.c

clean:
	rm -rf *.o *~ *.x