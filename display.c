#define DISPLAY
#include "display.h"
#include <stdlib.h>

screen initterm(){
	initscr();
	noecho();
	if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	screen temp;
	

	getmaxyx(stdscr, temp.height, temp.width);

	return temp;
}
window inittop(){
	window temp;
	temp.height = term.height - SPACE; 
	temp.width = term.width;

	temp.self = add_window(temp.height, temp.width, 0,0);
	scrollok(temp.self,TRUE); 
	return temp;
}

window initstatus(){
	window temp;
	temp.height = SPACE;
	temp.width = term.width;
	start_color();
	temp.self = add_window(temp.height, temp.width, term.height - SPACE,0);
	return temp;
}

void initcolors(){
	init_pair(color_reg.number, color_reg.fg, color_reg.bg);
	init_pair(color_error.number, color_error.fg, color_error.bg);
	init_pair(color_warning.number, color_warning.fg, color_warning.bg);
	init_pair(color_success.number, color_success.fg, color_success.bg);
}

WINDOW *add_window(int height, int width, int y, int x)
{	WINDOW *local_win;
	local_win = newwin(height, width, y, x);
	wrefresh(local_win);
	return local_win;
}

void serror(char *string)
{
	wclear(status.self);
	pstatus(color_error.number, string);
}

void slog(char *string)
{
	wclear(status.self);
	pstatus(color_reg.number, string);
}

void ssuccess(char *string)
{
	wclear(status.self);
	pstatus(color_success.number, string);
}
void pstatus(int type, char *string)
{
	wattron(status.self, COLOR_PAIR(type));
	vpstatus(top, "%s", string);
	wattroff(status.self, COLOR_PAIR(type));
}

void vpstatus(window return_window, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	vw_printw(status.self, fmt, ap);
	wrefresh(status.self);
	setlast();
	wmove(return_window.self, return_window.y, return_window.x);
	wrefresh(status.self);

	va_end(ap);
}

void tlog(char *string)
{
	cvptop(color_warning.number, "%s", string);
}
void cvptop(int type, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	wattron(top.self, COLOR_PAIR(type));
	vw_printw(top.self, fmt, ap);
	wattroff(top.self, COLOR_PAIR(type));

	va_end(ap);

	setlast();
}
void vptop(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vw_printw(top.self, fmt, ap);
	va_end(ap);

	setlast();
}
xy getcords(WINDOW *win) {
	xy cords;
	getyx(win, cords.y, cords.x);
	return cords;
}
void setlast(){
	xy cords = getcords(top.self); 
	top.y = cords.y; 
	top.x = cords.x;
}

void dprint(int index, int sel, int size, char *path){
	int cindex, csize, cpath;
	if(sel){
		cindex = color_success.number;
		csize = color_warning.number;
		cpath = color_success.number;
	}
	else{
		cindex = color_warning.number;
		csize = color_warning.number;
		cpath = color_warning.number;
	}

	cvptop(cindex, "- %d: ", index);
	cvptop(csize, "%d ", size);
	cvptop(cpath, "%s ", path );
	cvptop(cpath, "\n");
}
void dprintls(int index, int size, char *path){
	dprint(index, 1, size, path);
}
void dprintl(int index, int size, char *path){
	dprint(index, 0, size, path);
}
