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
void initcolors(void);

typedef struct color_pair {
	int number;
	int fg;
	int bg;
} color_pair;

#ifdef DISPLAY

color_pair color_reg = { 1, COLOR_WHITE, COLOR_BLACK };
color_pair color_warning = { 2, COLOR_YELLOW, COLOR_BLACK };
color_pair color_error = { 3, COLOR_RED, COLOR_BLACK };
color_pair color_success = { 3, COLOR_GREEN, COLOR_BLACK };

#endif


WINDOW *add_window(int height, int width, int y, int x);
xy getcords(WINDOW *win); 

void ptop(int type, char *string);
void vptop(const char *fmt, ...);
void tlog(char *string);

void pstatus(window return_window, int type, char *string);
void vpstatus(window return_window, const char *fmt, ...);
void serror(char *string);
void slog(char *string);
void ssuccess(char *string);
void setlast(void);
