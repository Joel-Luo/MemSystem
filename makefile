SHELL = /usr/bin/bash

all:	MemSystem
CC = g++
CFLAGS = -O3 -Wall

SRC=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

MemSystem:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o MemSystem

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o
