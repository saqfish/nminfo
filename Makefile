CC=gcc
CFLAGS=-lncurses

nminfo: nminfo.o display.o dir.o
	$(CC) $(CFLAGS) -o nminfo nminfo.o display.o dir.o
	
nminfo.o: nminfo.c
	$(CC) -c nminfo.c 

display.o: display.c
	$(CC) -c display.c

dir.o: dir.c 
	$(CC) -c dir.c
clean:
	rm *.o nminfo
