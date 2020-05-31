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
	while((ch = wgetch(status.self)) != 'q')
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
			case 'h':
				seldir();
				break;
			case 'v':
				setmode(mode_multi);
				break;
			case 'd':
				setmode(mode_delete);
				break;
		}
		if(dmode == mode_multi.number){
			dirs[dirsel].selected = dirs[dirsel].selected == TRUE ? FALSE : TRUE;
		}
		initmenu();
		wrefresh(top.self);
	}

	cleanup_dir();
	cleanup_display();
	free(dirs);
	endwin();
}
