#include <curses.h>

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
