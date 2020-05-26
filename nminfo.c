#define _GNU_SOURCE
#include <stdarg.h>

#include <panel.h>

#include "nminfo.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <ftw.h>

window top;
window status;
screen term;

static int addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

int main(int argc, char *argv[])
{
	term = initterm();
	top = inittop();
	status = initstatus();
	initcolors();

	ssuccess("Initialized");

	getmodules(argc, argv);

	int ch;
	while((ch = wgetch(top.self)) != 'q')
	{
		switch(ch){
			case 'c':
				for(int i = 0; i < 5; i++) {
					tlog("testing");
					tlog("\n");
				}
				slog("5 testings added");
				break;
			case 'v':
				vpstatus(top,"y: %d x: %d", top.y, top.x);
				break;
		}
	}
	endwin();
}


int getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;

	if (nftw((argc < 2) ? "." : argv[1], addmodules, 20, flags) == -1)
	{
		serror("nftw");
		exit(EXIT_FAILURE);
	}
}
static int
addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	const char *dircheck = "node_modules";
	if(tflag == FTW_D){
		const char *filename = fpath + ftwbuf->base;
		if(strcmp(filename, dircheck) == 0){
			vptop("%d %s\n", sb->st_size, fpath + 1);
			return FTW_SKIP_SIBLINGS;
		}
	}
	return 0;
}


