#ifndef FREE_H
#define FREE_H

#include "image.h"

void set_free(unsigned char *block, int num, int set);

int find_free(unsigned char *block);

#endif