# Makefile for whm.

CC=gcc
MODEFLAGS=-DMAINTAINER_MODE
CFLAGS=-Wall `pkg-config --cflags libglade-2.0 libgda` `xml2-config --cflags` $(MODEFLAGS) #-g
LDFLAGS=`pkg-config --libs libglade-2.0 libgda` `xml2-config --libs`
OBJS=main.o callbacks.o interface.o engine.o parser.o util.o
TARG=whm

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

callbacks.o: callbacks.c callbacks.h
	$(CC) -c $(CFLAGS) callbacks.c

interface.o: interface.c interface.h
	$(CC) -c $(CFLAGS) interface.c

engine.o: engine.c datagui.h engine.h parser.h
	$(CC) -c $(CFLAGS) engine.c

parser.o: parser.c datagui.h parser.h util.h
	$(CC) -c $(CFLAGS) parser.c

util.o: util.c util.h
	$(CC) -c $(CFLAGS) util.c

.PHONY: clean
clean:
	rm -f $(TARG) $(OBJS)
