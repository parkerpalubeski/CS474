#include "block.h"
#include "image.h"

unsigned char *bread(int block_num, unsigned char *block){
    if(block_num < 0){
        perror("bread: block_num value");
        return NULL; //NULL is error value instead of -1
    }
    int offset = block_num * BLOCK_SIZE;
    if(lseek(image_fd, offset, SEEK_SET) == -1){
        perror("bread: lseek");
        return NULL;
    }
    if(read(image_fd, block, BLOCK_SIZE) == -1){
        perror("bread: read");
        return NULL;
    }
    return block;
}

void bwrite(int block_num, unsigned char *block){
    int offset = block_num * BLOCK_SIZE;
    if(lseek(image_fd, offset, SEEK_SET) == -1){
        perror("bwrite: lseek");
    }
    if(write(image_fd, block, BLOCK_SIZE) == -1){
        perror("bwrite: write");
    }
}