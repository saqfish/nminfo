#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include "dir.h"
#include "display.h"


void listdirs(int sel)
{
	wclear(top.self);
	int min;
	int max;
	if(dirsize > top.height) {
		if(	dirsel <= (dirsize - top.height)){
			min = dirsel;
		}else{
			min = (dirsize - top.height);
		}
		int excess = (dirsize-top.height) - dirsel;
		max = excess > 0 ? dirsize - excess : dirsize;
	}else{
		min = 0;
		max = dirsize;
	}
	// vpstatus(top,"height: %d min: %d max: %d",top.height, min,max);
	for(int i = min; i < max; i++){ 
		if(i == dirsel)
			dprintls(i+1, dirs[i].size, dirs[i].path);
		else
			dprintl(i+1, dirs[i].size, dirs[i].path);
	}
}

void nextdir()
{
	dirsel = dirsel < (dirsize -1) ? (dirsel + 1) : dirsize - 1;
	listdirs(dirsel);
}
void prevdir()
{
	dirsel = dirsel > 0 ? (dirsel - 1): 0; 
	listdirs(dirsel);
}
void seldir()
{
	serror(dirs[dirsel].path);
}
int getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;

	if (nftw((argc < 2) ? "." : argv[1], addmodules, 20, flags) == -1)
	{
		serror("nftw");
	}else listdirs(dirsel);
}

	int
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
