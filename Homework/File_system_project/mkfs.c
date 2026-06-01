// Makes the file system to disk
#include "mkfs.h"

// initializes the image file
// also initializes the root directory at inode #0
// args : none
// returns : none
void mkfs(void)
{
    unsigned char block[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
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

    // create the root directory
    struct inode *root = ialloc();
    int dir_entries = alloc();
    root->flags = 2;
    root->size = 2 * RECORD_SIZE;
    root->block_ptr[0] = dir_entries;

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        block[i] = 0;
    }
    // create .
    int offset1 = 0;
    write_u16(block + offset1, root->inode_num);
    block[offset1 + 2] = '.';
    block[offset1 + 3] = '\0';
    // create ..
    int offset2 = RECORD_SIZE;
    write_u16(block + offset2, root->inode_num);
    block[offset2 + 2] = '.';
    block[offset2 + 3] = '.';
    block[offset2 + 4] = '\0';
    bwrite(dir_entries, block);

    iput(root);
    (void)dir_entries;
}