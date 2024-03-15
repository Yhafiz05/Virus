CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

MediaPlayer: MediaPlayer.c
	$(CC) -o MediaPlayer MediaPlayer.c $(CFLAGS) $(LIBS)
