#ifndef DIRBASENAME_H
#define DIRBASENAME_H

#include <stdio.h>
#include <string.h>

char *get_dirname(const char *path, char *dirname);

char *get_basename(const char *path, char *basename);

int test(void);

#endif