#include "display.h"

screen initterm(){
	initscr();
	start_color();
	noecho();

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

	temp.self = add_window(temp.height, temp.width, term.height - SPACE,0);
	return temp;
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
