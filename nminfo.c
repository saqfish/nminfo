#define _GNU_SOURCE
#include <stdarg.h>

#include <curses.h>
#include <panel.h>

#include "nminfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

xy getcords(WINDOW *win); 
screen term;
window top;
window status;

int main(int argc, char *argv[])
{
	initscr();
	start_color();
	noecho();

	getmaxyx(stdscr, term.height, term.width);

	int TOP_HEIGHT = term.height - 3;
	int TOP_WIDTH = term.width;
	int TOP_Y = 0;
	int TOP_X = 0;

	int STATUS_HEIGHT = 3;
	int STATUS_WIDTH = term.width;
	int STATUS_Y = term.height - 3;
	int STATUS_X = 0;

	top.height = TOP_HEIGHT; 
	top.width = TOP_WIDTH;
	top.y = TOP_Y; 
	top.x = TOP_X;

	status.height = STATUS_HEIGHT;
	status.width =STATUS_WIDTH;
	status.y = STATUS_Y;
	status.x = STATUS_X;


	top.self = add_window(top.height, top.width, top.y,top.x);
	scrollok(top.self,TRUE); 

	status.self = add_window(status.height, status.width, status.y,status.x);

	pstatus(top, "Init");

	getmodules(argc, argv);

	int ch;
	while((ch = wgetch(top.self)) != 'q')
	{
		switch(ch){
			case 'c':
				for(int i = 0; i < 5; i++) {
					vptop("%d %s\n", 5, "testing");
				}
				pstatus(top, "5 testings added");
				break;
			case 'v':
				vpstatus(top, "y: %d x: %d", top.y, top.x);
				break;
		}
	}
	endwin();
}

WINDOW *add_window(int height, int width, int y, int x)
{	WINDOW *local_win;
	local_win = newwin(height, width, y, x);
	wrefresh(local_win);
	return local_win;
}

void pstatus(window return_window, char *string)
{
	vpstatus(return_window, "%s", string);
}

void vpstatus(window return_window, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	wclear(status.self);

	vw_printw(status.self, fmt, ap);
	wrefresh(status.self);
	setLast();
	wmove(return_window.self, return_window.y, return_window.x);
	wrefresh(status.self);

	va_end(ap);
}
void ptop(char *string)
{
	vptop("%s", string);
}
void vptop(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vw_printw(top.self, fmt, ap);
	va_end(ap);

	setLast();
}
xy getcords(WINDOW *win) {
	xy cords;
	getyx(win, cords.y, cords.x);
	return cords;
}
void setLast(){
	xy cords = getcords(top.self); 
	top.y = cords.y; 
	top.x = cords.x;
}

int getmodules(int argc, char *argv[]) {
	int flags = 0;

	flags |= FTW_ACTIONRETVAL;

	if (nftw((argc < 2) ? "." : argv[1], display_info, 20, flags) == -1)
	{
		pstatus(top,"nftw");
		exit(EXIT_FAILURE);
	}
}
static int
display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
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


