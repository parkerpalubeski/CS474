//Makes the file system to disk
#include "inode.h"
#include "ctest.h"

void initialize_system(void){
    unsigned char block[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        block[i] = 0;
    }
    bwrite(SUPERBLOCK, block);
    bwrite(INODE_MAP, block);
    bwrite(INODE0, block);
    bwrite(INODE1, block);
    bwrite(INODE2, block);
    bwrite(INODE3, block);

    block[0] = 0x7f;
    bwrite(BLOCK_MAP, block);
}

int main(){
    image_open("image.img", 1);
    initialize_system();
}