CC      = gcc
CFLAGS  = -Wall -Werror 
LDFLAGS = 
RM      = rm -f
TARG    = wish
OBJFILES = wish.o main.o
BIN     = bin/

default: all

all: $(TARG)

$(TARG): $(OBJFILES)
	$(CC) $(CFLAGS) -g -o $(BIN)$(TARG) $(OBJFILES) $(LDFLAGS)

clean veryclean:
	$(RM) $(OBJFILES) $(OUT)
