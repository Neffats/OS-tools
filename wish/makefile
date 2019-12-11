CC      = gcc
CFLAGS  = -Wall -Werror 
RM      = rm -f
TARG    = wish
SRCS    = wish.c

default: all

all: $(TARG)

$(TARG): $(SRCS)
	$(CC) -g -o $(TARG) $(SRCS) $(CFLAGS)

clean veryclean:
	$(RM) $(OUT)
