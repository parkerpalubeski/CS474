#include "dir.h"

//opens a directory via its inode number
//arguments: int
//returns: a pointer to the open directory
struct directory *directory_open(int inode_num){
    struct directory* dir = (struct directory*)malloc(sizeof(struct directory));
    dir->inode = iget(inode_num);
    if(dir->inode == NULL){
        return NULL;
    }
    dir->offset = 0;
    return dir;
}

//reads data from a directory into a directory entry
//arguments: struct directory, struct directory_entry
//returns: int for error checking, string and int both via pointer
int directory_get(struct directory *dir, struct directory_entry *ent){
    if(dir->offset >= dir->inode->size){
        return -1;
    }
    int data_block_index = dir->offset/BLOCK_SIZE;

    unsigned char buf[BLOCK_SIZE];
    bread(dir->inode->block_ptr[data_block_index], buf);
    int block_offset  = dir->offset % BLOCK_SIZE;

    unsigned char *record = buf + block_offset;

    ent->inode_num = read_u16(record);
    strcpy(ent->name, (char*) record + 2);

    dir->offset += RECORD_SIZE; //program infinite looped without this

    return 0;
}

//frees up a directory's inode and the malloc'd struct
//arguments: struct directory
//returns: none
void directory_close(struct directory *d){
    iput(d->inode);
    free(d);
}