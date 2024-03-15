CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

devinette: devinette.c
	$(CC) -o devinette devinette.c $(CFLAGS) $(LIBS)
