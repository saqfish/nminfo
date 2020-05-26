#include <curses.h>

#define SPACE 3

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

extern screen term;
extern window top;
extern window status;

screen initterm();
window inittop();
window initstatus();
WINDOW *add_window(int height, int width, int y, int x);
void setLast(void);
void ptop(char *string);
void vptop(const char *fmt, ...);
void pstatus(window return_window, char *string);
void vpstatus(window return_window, const char *fmt, ...);
xy getcords(WINDOW *win); 

