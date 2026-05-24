#ifndef IMAGE_H
#define IMAGE_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

int image_open(char *filename, int truncate);
int image_close(void);

extern int image_fd;

#endif