#include <ftw.h>
#include <menu.h>

#ifndef EXTERN_DIR 
#define EXTERN_DIR extern
#endif

typedef struct package {
	long int size;
	int depth;
	char *path;
} package;

typedef struct dir {
	long int size;
	int depth;
	char *projectname;
	char *parentname;
	int numpackages;
	char *path;
	struct dir *packages;
} dir;


EXTERN_DIR dir *dirs;
EXTERN_DIR int dirsize;
EXTERN_DIR int dirsel;
EXTERN_DIR MENU *dirmenu;


int getmodules(int argc, char *argv[]);
int addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);
char * getdirname(char *path, int level);

void dprint(int index, int sel,int size, char *parent, char *name);
void dprintl(int index, int size, char *parent, char *name);
void dprintls(int index, int size, char *parent, char *name);
void dsprint(int depth, char *path);

void listdirs(int sel, int inc);
void nextdir();
void prevdir();
void seldir();
