CC      = gcc
CFLAGS  = -Wall -Werror
RM      = rm -f


default: all

all: wcat

wcat: wcat.c
	$(CC) -o wcat wcat.c $(CFLAGS)

clean veryclean:
	$(RM) Hello