#include <curses.h>

#ifndef EXTERN_DISPLAY 
#define EXTERN_DISPLAY extern
#endif

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

EXTERN_DISPLAY screen term;
EXTERN_DISPLAY window top;
EXTERN_DISPLAY window status;

screen initterm();
window inittop();
window initstatus();

void initcolors(void);

typedef struct color_pair {
	int number;
	int fg;
	int bg;
} color_pair;


EXTERN_DISPLAY color_pair color_reg; 
EXTERN_DISPLAY color_pair color_warning;
EXTERN_DISPLAY color_pair color_error;
EXTERN_DISPLAY color_pair color_success;

WINDOW *add_window(int height, int width, int y, int x);
xy getcords(WINDOW *win); 

void vptop(const char *fmt, ...);
void cvptop(int type, const char *fmt, ...);
void tlog(char *string);

void pstatus(int type, char *string);
void vpstatus(window return_window, const char *fmt, ...);
void serror(char *string);
void slog(char *string);
void ssuccess(char *string);
void setlast(void);
