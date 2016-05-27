all:	MemSystem clean
CC = g++
CFLAGS = -std=c++11


MemSystem:	*.o
	$(CC) $(CFLAGS) *.o -o MemSystem

*.o: *.cpp
	$(CC) $(CFLAGS) -c *.cpp

.PHONY: clean
clean:
	rm -f *.o
