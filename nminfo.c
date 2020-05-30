#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "nminfo.h"
#include "display.h"
#include "dir.h"

int 
main(int argc, char *argv[])
{
	dirs = NULL;
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
				seldir(0);
				break;
			case 'h':
				seldir(1);
				break;
		}
	}
	endwin();
}
