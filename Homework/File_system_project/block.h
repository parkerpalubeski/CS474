#ifndef BLOCK_H
#define BLOCK_H

#include "free.h"

unsigned char *bread(int block_num, unsigned char *block);
void bwrite(int block_num, unsigned char *block);
int alloc(void);

#endif