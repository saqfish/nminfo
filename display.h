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

EXTERN_DISPLAY const color_pair color_reg; 
EXTERN_DISPLAY const color_pair color_warning;
EXTERN_DISPLAY const color_pair color_error;
EXTERN_DISPLAY const color_pair color_success;
EXTERN_DISPLAY const color_pair color_selected;

typedef struct mode {
	int number;
} mode;

EXTERN_DISPLAY int dmode;
EXTERN_DISPLAY const mode mode_delete;
EXTERN_DISPLAY const mode mode_multi;
EXTERN_DISPLAY const mode mode_browse;

void setmode(mode mode);
int getmode();

WINDOW *add_window(int height, int width, int y, int x);
xy getcords(WINDOW *win); 

void pstatus(int type, char *string);
void vpstatus(const char *fmt, ...);
void serror(char *string);
void slog(char *string);
void ssuccess(char *string);

void cleanup_display(void);
