all:	MemSystem rmobj
CC = g++
CFLAGS = -std=c++11 -g -Wall


MemSystem:	*.o 
	$(CC) $(CFLAGS) *.o -o MemSystem

*.o: *.cpp
	$(CC) $(CFLAGS) -c *.cpp

.PHONY: clean
clean:
	rm -f *.o MemSystem
rmobj:
	rm -f *.o       
