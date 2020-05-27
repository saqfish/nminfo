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

void listdirs(int sel);
void nextdir();
void prevdir();

typedef struct dir {
	long int size;
	char *path;
} dir;

dir *dirs;
int dirsize = 0;
int dirsel = 0;

void listdirs(int sel)
{
	wclear(top.self);
	for(int i = 0; i< dirsize; i++){ 
		int index = i + 1;
		if(i == dirsel)
			dprintls(index, dirs[i].size, dirs[i].path);
		else
			dprintl(index, dirs[i].size, dirs[i].path);
	}
}

void nextdir()
{
	dirsel = dirsel < (dirsize -1) ? (dirsel + 1) : 0;
	listdirs(dirsel);
}
void prevdir()
{
	dirsel = dirsel <= (dirsize - 1) && dirsel > 0? (dirsel - 1) : dirsize - 1;
	listdirs(dirsel);
}
int getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;

	if (nftw((argc < 2) ? "." : argv[1], addmodules, 20, flags) == -1)
	{
		serror("nftw");
		exit(EXIT_FAILURE);
	}else listdirs(dirsel);
	vpstatus(top,"Dirs: %d",dirsize);
}

static int
addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	const char *dircheck = "node_modules";
	int length = sizeof(dirs);
	if(tflag == FTW_D){
		const char *filename = fpath + ftwbuf->base;
		if(strcmp(filename, dircheck) == 0){
			dir directory = {sb->st_size};
			directory.path = malloc(strlen(fpath)+1);
			strcpy(directory.path, fpath);

			dirs[dirsize++] = directory;
			return FTW_SKIP_SIBLINGS;
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	dirs = malloc(20 * sizeof(dir));
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
		}
	}
	endwin();
}


