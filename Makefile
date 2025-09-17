CC = gcc -std=gnu99
OPTS = -Wall -Werror
DEBUG = -g

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./inc

INCLUDE=$(addprefix -I,$(INCDIR))
CFLAGS=$(OPTS) $(INCLUDE) $(DEBUG)
OBJECTS=$(addprefix $(OBJDIR)/,mngr.o exec.o)

all: mngr

mngr: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
$(OBJDIR)/mngr.o: $(SRCDIR)/mngr.c $(INCDIR)/exec.h $(INCDIR)/listnode.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm mngr
	rm $(OBJDIR)/mngr.o
