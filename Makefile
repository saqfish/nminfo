CC=gcc
CFLAGS=-lncurses

nminfo: nminfo.o display.o
	$(CC) $(CFLAGS) -o nminfo nminfo.o display.o
	
nminfo.o: nminfo.c nminfo.h display.h
	$(CC) -c nminfo.c 

display.o: display.c display.h
	$(CC) -c display.c
clean:
	rm *.o
