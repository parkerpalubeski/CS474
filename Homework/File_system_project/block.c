//Contains functions to read and write to blocks of memory in the image file

#include "block.h"

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

//Writes to an entire block
void bwrite(int block_num, unsigned char *block){
    int offset = block_num * BLOCK_SIZE;
    if(lseek(image_fd, offset, SEEK_SET) == -1){
        perror("bwrite: lseek");
    }
    if(write(image_fd, block, BLOCK_SIZE) == -1){
        perror("bwrite: write");
    }
}

int alloc(void){
    unsigned char buf[BLOCK_SIZE];

    bread(BLOCK_MAP, buf); //BLOCK_MAP is defined as 2 in image.h

    int free_b = find_free(buf);
    if(free_b == -1){
        //MAP FULL : RETURN -1
        return 1;
    }

    set_free(buf, free_b, 1);
    bwrite(BLOCK_MAP, buf);


    return free_b;
}