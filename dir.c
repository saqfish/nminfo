#define _GNU_SOURCE
#define EXTERN_DIR

#include <stdlib.h>
#include <string.h>

#include "dir.h"
#include "display.h"

void 
dprint(int depth, int sel, int size, char *parent, char *name){
	int cindex, carrow, cname;

	cindex = sel ? color_success.number: color_warning.number;
	carrow = sel ? color_reg.number: color_reg.number;
	cname = sel ? color_success.number: color_warning.number;

	for(int i = 0; i< depth; i++) 
			cvptop(carrow, "--");

	cvptop(cindex, "%s", parent);
	cvptop(carrow, "->");
	cvptop(cindex, "%s ", name);
	cvptop(cname, "\n");
}

char *
getdirname(char *path, int depth){
	char *rest; 
	char *token; 
	char *ptr = malloc(strlen(path)+1);
	strcpy(ptr, path);

	int count = 1;
	while(token = strtok_r(ptr, "//", &rest)) {
		if(count == depth) {
			return token;
		}
		ptr = rest;
		count++;
	}
	return "";
}

void 
dtprintls(int depth, int size, char *parent, char *name){
	dprint(depth, 1, size, parent, name);
}

void 
dtprintl(int depth, int size, char *parent, char *name){
	dprint(depth, 0, size, parent, name);
}

void 
dsprint(int depth, char *path){
	vpstatus(top, "--- %d: ", depth);
	vpstatus(top, "%s ", path );
	vpstatus(top, "\n");
}

void 
listdirs(int sel)
{
	int min;
	int max;

	wclear(top.self);
	if(dirsize > top.height) {
		min = dirsel <= (dirsize - top.height) ? dirsel : (dirsize - top.height);
		int excess = (dirsize-top.height) - dirsel;
		max = excess > 0 ? dirsize - excess : dirsize;
	}else{
		min = 0;
		max = dirsize;
	}
	for(int i = min; i < max; i++){ 
		if(i == dirsel)
			dtprintls(dirs[i].depth, dirs[i].numpackages, dirs[i].parentname, dirs[i].projectname);
		else
			dtprintl(dirs[i].depth, dirs[i].numpackages, dirs[i].parentname, dirs[i].projectname);
	}
}

int 
getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;
	if (nftw((argc < 2) ? "." : argv[1], addmodules, 1, flags) == -1)
	{
		serror("nftw");
	}else listdirs(dirsel);
}

int 
getdepth(const char *fpath) {
	int count = 0;
	int len = strlen(fpath);
	char *copy;
        copy = malloc(len*sizeof(char *));
	strcpy(copy, fpath);

	while (copy = strstr(copy, "node_modules")){
		count++;
		copy++;
	}
	free(copy);
	return count;
}

int
addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	const char *dircheck = "node_modules";
	if(tflag == FTW_D){
		const char *filename = fpath + ftwbuf->base;
		if(strcmp(filename, dircheck) == 0){
			int depth = getdepth(fpath);
			if(dirsize == 0) 
				dirs = malloc(1 * sizeof(dir));
			else
				dirs = realloc(dirs, (dirsize + 1) * sizeof(dir));

			dir directory = {sb->st_size, depth};
			directory.path = malloc(strlen(fpath)+1);
			strcpy(directory.path, fpath);

			directory.projectname = getdirname(directory.path, (depth*2));
			directory.parentname = getdirname(directory.path, (depth*2)-2);

			/*
			./x!/n 1 (2) 2
			./x!/n/x/n 2 (4) 2
			./x/n/x!/n/x/n 3 (6) 4
			./x/n/x/n/x!/n/x/n 4 (8) 6
			./x/n/x/n/x/n/x!/n/x/n 5 (8) 6
			*/

			directory.packages = NULL;
			directory.numpackages = 0;
			dirs[dirsize] = directory;
			dirsize++;
			return FTW_SKIP_SUBTREE;
		}
	}
	return 0;
}


void 
nextdir()
{
	dirsel = dirsel < (dirsize -1) ? (dirsel + 1) : dirsize - 1;
	listdirs(dirsel);
}

void 
prevdir()
{
	dirsel = dirsel > 0 ? (dirsel - 1): 0; 
	listdirs(dirsel);
}

void 
seldir()
{
	wclear(status.self);
	dsprint(dirs[dirsel].depth, dirs[dirsel].path);
}
