CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

temperature: temperature.c
	$(CC) -o temperature temperature.c $(CFLAGS) $(LIBS)
