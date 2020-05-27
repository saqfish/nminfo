#define _GNU_SOURCE
#define EXTERN_DIR
#define EXTERN_DISPLAY

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "nminfo.h"
#include "display.h"
#include "dir.h"

int 
main(int argc, char *argv[])
{
	dirs = malloc(30 * sizeof(dir));
	term = initterm();
	top = inittop();
	status = initstatus();
	initcolors();
	getmodules(argc, argv);

	int ch;
	while((ch = wgetch(top.self)) != 'q')
	{
		switch(ch){
			case 'c':
				break;
			case 'j':
				nextdir();
				break;
			case 'k':
				prevdir();
				break;
			case 'l':
				seldir();
				break;
		}
	}
	endwin();
}
