#include <ftw.h>

#ifndef EXTERN_DIR 
#define EXTERN_DIR extern
#endif

typedef struct dir {
	long int size;
	char *path;
} dir;


EXTERN_DIR dir *dirs;
EXTERN_DIR int dirsize;
EXTERN_DIR int dirsel;

void listdirs(int sel);
void nextdir();
void prevdir();
void seldir();

int getmodules(int argc, char *argv[]);
int addmodules(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

