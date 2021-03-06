#define _GNU_SOURCE
#define EXTERN_DIR

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dir.h"
#include "display.h"


ITEM **entries;

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
	free(ptr);
	return "";
}


void 
initmenu()
{
	ITEM *selitem;
	int topindex;
	int c,i;

	topindex = top_row(dirmenu);

	entries = (ITEM **)calloc(dirsize + 1, sizeof(ITEM *));
	for(i = 0; i < dirsize; ++i){
		entries[i] = new_item(dirs[i].path + 1, dirs[i].path);
		if(i == dirsel) selitem = entries[i];
		if(dirs[i].selected) 
			item_opts_off(entries[i], O_SELECTABLE);
	}

	dirmenu = new_menu((ITEM **)entries);

	menu_opts_off(dirmenu, O_SHOWDESC);

	set_menu_sub(dirmenu, derwin(top.self, top.height, top.width, 0, 0));
	set_menu_format(dirmenu, top.height, 1);

	set_menu_fore(dirmenu, COLOR_PAIR(getmode()) | A_REVERSE);
	set_menu_back(dirmenu, COLOR_PAIR(color_reg.number));
	set_menu_grey(dirmenu, COLOR_PAIR(color_warning.number) | A_REVERSE);


	post_menu(dirmenu);
	wrefresh(top.self);

	set_top_row(dirmenu, topindex);
	set_current_item(dirmenu, selitem);

	free_item(selitem);
}

int 
getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;
	vpstatus("Scanning for node_modules.. \n");

	clock_t begin = clock();
	if (nftw((argc < 2) ? "." : argv[1], addmodules, 1, flags) == -1)
	{
		serror("nftw");
	}else {
		initmenu();
	}
	clock_t end = clock();

	double duration = (double)(end - begin) / CLOCKS_PER_SEC;
	vpstatus("%d modules. Done in: %f seconds\n", dirsize, duration);
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

			int realdepth = ftwbuf->level;

			dir directory = {sb->st_size, realdepth };
			directory.path = malloc(strlen(fpath)+1);
			strcpy(directory.path, fpath);

			directory.selected = FALSE;

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
}

void 
prevdir()
{
	dirsel = dirsel > 0 ? (dirsel - 1): 0; 
	menu_driver(dirmenu, REQ_UP_ITEM);
}

void 
seldir()
{
	if(dmode == mode_multi.number){
		dirs[dirsel].selected = TRUE;
	}
}


void 
cleanup_dir(){
	free_menu(dirmenu);
	for(int i = 0; i < dirsize; ++i){
		free_item(entries[i]);
	}
}
