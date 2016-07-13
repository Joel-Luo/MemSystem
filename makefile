all:	MemSystem rmobj
CC = g++
CFLAGS = -std=c++11 -Wall -O1


MemSystem:	*.o 
	$(CC) $(CFLAGS) *.o -o MemSystem

*.o: *.cpp
	$(CC) $(CFLAGS) -c *.cpp

DEBUG: MemSystem_G rmobj
	
MemSystem_G:	COMPILE_G
	$(CC) $(CFLAGS) -g *.o -o MemSystem_G

COMPILE_G:
	$(CC) $(CFLAGS) -g -c *.cpp

.PHONY: clean
clean:
	rm -f *.o MemSystem Mem_System_G
rmobj:
	rm -f *.o       
