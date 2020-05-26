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
	pstatus(top,color_error.number, string);
}

void slog(char *string)
{
	pstatus(top,color_reg.number, string);
}

void ssuccess(char *string)
{
	pstatus(top,color_success.number, string);
}
void pstatus(window return_window, int type, char *string)
{
	wattron(status.self, COLOR_PAIR(type));
	vpstatus(return_window, "%s", string);
	wattroff(status.self, COLOR_PAIR(type));
}

void vpstatus(window return_window, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	wclear(status.self);

	vw_printw(status.self, fmt, ap);
	wrefresh(status.self);
	setlast();
	wmove(return_window.self, return_window.y, return_window.x);
	wrefresh(status.self);

	va_end(ap);
}

void tlog(char *string)
{
	ptop(color_warning.number, string);
}
void ptop(int type, char *string)
{
	wattron(top.self, COLOR_PAIR(type));
	vptop("%s", string);
	wattroff(top.self, COLOR_PAIR(type));
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
