#define _GNU_SOURCE
#define EXTERN_DIR

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dir.h"
#include "display.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


void 
dprint(int depth, int sel, int size, char *parent, char *name){
	int cindex, carrow, cname;
	char *arrow = "-->";
	char *newline = "\n";

	cindex = sel ? color_selected.number: color_warning.number;
	carrow = sel ? color_selected.number: color_warning.number;
	cname = sel ? color_selected.number: color_warning.number;

	int len = 0;

	for(int i = 0; i< depth; i++) {
		cvptop(carrow, "--");
		len += 2;
	}

	len += strlen(parent);
	cvptop(cindex, "%s", parent);

	len += strlen(arrow);
	cvptop(carrow, arrow);

	len += strlen(name);
	cvptop(cindex, "%s ", name);

	if(len < top.width)
		for(int i = len; i < top.width - strlen(newline) - 1; i++)
			cvptop(cindex, " ", name);

	cvptop(cname, newline);
}

char *
getdirname(char *path, int depth){
	char *rest; 
	char *token; 
	char *ptr = malloc(strlen(path)+1);
	strcpy(ptr, path);

	int count = 0;
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
listdirs(int sel, int inc)
{
	ITEM **entries;
	int c,i;

	entries = (ITEM **)calloc(dirsize + 1, sizeof(ITEM *));
	for(i = 0; i < dirsize; ++i)
		entries[i] = new_item(dirs[i].projectname ,dirs[i].parentname);

	dirmenu = new_menu((ITEM **)entries);

	set_menu_sub(dirmenu, derwin(top.self, top.height, top.width, 0, 0));
	set_menu_format(dirmenu, top.height, 1);

	set_menu_mark(dirmenu, " -- ");

	post_menu(dirmenu);
	wrefresh(top.self);

}

int 
getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;
	vpstatus(top, "Scanning for node_modules.. \n");

	clock_t begin = clock();
	if (nftw((argc < 2) ? "." : argv[1], addmodules, 1, flags) == -1)
	{
		serror("nftw");
	}else {
		listdirs(dirsel, 1);
	}
	clock_t end = clock();

	double duration = (double)(end - begin) / CLOCKS_PER_SEC;
	vpstatus(top, "%d modules. Done in: %f seconds\n", dirsize, duration);
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

char *
getparents(char *path){
	char *rest; 
	char *token; 
	char *ptr = malloc(strlen(path)+1);
	strcpy(ptr, path);

	char *parents;
	char *arrow = "-->";

	int count = 0;
	while(token = strtok_r(ptr, "//", &rest)) {
		if(strcmp("node_modules", token) != 0) {
			if(count == 0) {
				parents = malloc(strlen(token)+1);
				strcpy(parents, token);
			}
			else {
				parents = realloc(parents, strlen(parents) + strlen(arrow)+1);
				strcat(parents, arrow);

				parents = realloc(parents, strlen(parents) + strlen(token)+1);
				strcat(parents, token);
			}

			
		}
		ptr = rest;
		count++;
	}
	return parents;
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

			int realdepth = ftwbuf->level;

			dir directory = {sb->st_size, realdepth - depth};
			directory.path = malloc(strlen(fpath)+1);
			strcpy(directory.path, fpath);

			directory.projectname = getdirname(directory.path, (realdepth - 1));
			directory.parentname = getparents(directory.path);

			dirs[dirsize] = directory;
			dirsize++;
			return 0;
		}
	}
	return 0;
}


void 
nextdir()
{
	dirsel = dirsel < (dirsize -1) ? (dirsel + 1) : dirsize - 1;
	menu_driver(dirmenu, REQ_DOWN_ITEM);
	wrefresh(top.self);

}

void 
prevdir()
{
	dirsel = dirsel > 0 ? (dirsel - 1): 0; 
	menu_driver(dirmenu, REQ_UP_ITEM);
	wrefresh(top.self);
}

void 
seldir()
{
	wclear(status.self);
	dsprint(dirs[dirsel].depth, dirs[dirsel].path);
}
