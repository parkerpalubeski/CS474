#ifndef IMAGE_H
#define IMAGE_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

//added special block definitions
#define SUPERBLOCK          0
#define INODE_MAP           1
#define BLOCK_MAP           2
#define INODE0              3
#define INODE1              4
#define INODE2              5
#define INODE3              6
#define BLOCK_SIZE          4096
#define INODE_SIZE          64
#define INODES_PER_BLOCK    (BLOCK_SIZE/INODE_SIZE)

int image_open(char *filename, int truncate);
int image_close(void);

extern int image_fd;

#endif