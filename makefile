SHELL = /usr/bin/bash

all:	MemSystem clean	
CC = g++	
CFLAGS = -O3 -Wall

.SUFFIXS: .cpp
MemSystem:	%.o
	$(CC) $(CFLAGS) %.o -o MemSystem

%.o: %.cpp
	$(CC) $(CFLAGS) -o %.o -c %.cpp
.PHONY: clean
clean: 
	rm -f *.o
