#include <stdarg.h>

#include <curses.h>
#include <panel.h>

WINDOW *add_window(int height, int width, int y, int x);
void setLast(void);

void ptop(char *string);
void vptop(char *fmt, ...);

typedef struct screen {
	int height;
	int width;
} screen;

typedef struct window {
	WINDOW *self;
	int height;
	int width;
	int y;
	int x;
} window;

typedef struct xy {
	int y;
	int x;
} xy;

void pstatus(window return_window, char *string);
void vpstatus(window return_window, char *string, ...);

xy getcords(WINDOW *win); 
screen term;
window top;
window status;

int main()
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

	pstatus(top, "init");

	int ch;
	while((ch = wgetch(top.self)) != 'q')
	{
		switch(ch){
			case 'c':
				for(int i = 0; i < 5; i++) {
					vptop("%s\n", "testing");
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

void vpstatus(window return_window, char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);

	wclear(status.self);

	vw_printw(status.self, fmt, list);
	wrefresh(status.self);
	setLast();
	wmove(return_window.self, return_window.y, return_window.x);
	wrefresh(status.self);

	va_end(list);
}
void ptop(char *string)
{
	vptop("%s", string);
}
void vptop(char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	vw_printw(top.self, "%s\n", list);
	setLast();
	va_end(list);
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
