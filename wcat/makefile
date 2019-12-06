CC      = gcc
CFLAGS  = -Wall -Werror -lm
RM      = rm -f
OUT     = wcat


default: all

all: $(OUT)

wcat: wcat.c
	$(CC) -o $(OUT) wcat.c $(CFLAGS)

clean veryclean:
	$(RM) $(OUT)
