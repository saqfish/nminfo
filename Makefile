CC=gcc
CFLAGS=-lncurses

nminfo: nminfo.o 
	$(CC) $(CFLAGS) -o nminfo nminfo.o
	
nminfo.o: nminfo.c nminfo.h
	$(CC) -c nminfo.c -o nminfo.o 

clean:
	rm *.o
