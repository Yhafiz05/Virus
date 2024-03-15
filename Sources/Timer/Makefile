CC=gcc
CFLAGS	=	$(shell pkg-config --cflags gtk+-3.0)
LIBS	=	$(shell pkg-config --libs gtk+-3.0)

timer: timer.c
	$(CC) -o timer timer.c $(CFLAGS) $(LIBS)