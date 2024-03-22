CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

carre: carre.c
	$(CC) -o carre carre.c $(CFLAGS) $(LIBS)