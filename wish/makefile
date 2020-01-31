CC      = gcc
CFLAGS  = -Wall -Werror 
RM      = rm -f
TARG    = wish
SRCS    = wish.c
BIN     = bin/

default: all

all: $(TARG)

$(TARG): $(SRCS)
	$(CC) -g -o $(BIN)$(TARG) $(SRCS) $(CFLAGS)

clean veryclean:
	$(RM) $(OUT)
