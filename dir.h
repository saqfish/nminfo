#include <ftw.h>
#include <menu.h>

#ifndef EXTERN_DIR 
#define EXTERN_DIR extern
#endif

typedef struct dir {
	long int size;
	int depth;
	char *projectname;
	char *parentname;
	char *path;
} dir;


EXTERN_DIR dir *dirs;
EXTERN_DIR int dirsize;
EXTERN_DIR int dirsel;
EXTERN_DIR MENU *dirmenu;


int getmodules(int argc, char *argv[]);
int addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);
char * getdirname(char *path, int level);

void listdirs(int sel, int inc);
void nextdir();
void prevdir();
void seldir();
void setmode();

void cleanup_dir();
