#include "dir.h"
#include "dirbasename.c"

// opens a directory via its inode number
// arguments: int : the inode number of the opened directory
// returns: a pointer to the open directory
struct directory *directory_open(int inode_num)
{
    struct directory *dir = (struct directory *)malloc(sizeof(struct directory));
    dir->inode = iget(inode_num);
    if (dir->inode == NULL)
    {
        return NULL;
    }
    dir->offset = 0;
    return dir;
}

// reads data from a directory into a directory entry
// arguments: struct directory, struct directory_entry
// returns: int for error checking, string and int both via pointer
int directory_get(struct directory *dir, struct directory_entry *ent)
{
    if (dir->offset >= dir->inode->size)
    {
        return -1;
    }
    int data_block_index = dir->offset / BLOCK_SIZE;

    unsigned char buf[BLOCK_SIZE];
    bread(dir->inode->block_ptr[data_block_index], buf);
    int block_offset = dir->offset % BLOCK_SIZE;

    unsigned char *record = buf + block_offset;

    ent->inode_num = read_u16(record);
    strcpy(ent->name, (char *)record + 2);

    dir->offset += RECORD_SIZE; // program infinite looped without this

    return 0;
}

// frees up a directory's inode and the malloc'd struct
// arguments: struct directory
// returns: none
void directory_close(struct directory *d)
{
    iput(d->inode);
    free(d);
}

// returns the inode of the file at the file path passed in
// args : char* -> the file path
// returns : struct inode -> the inode of the file at path
struct inode *namei(char *path)
{
    (void)path;
    if(strcmp(path, "/") == 0){
        return iget(ROOT_INODE_NUM);
    }
    else{
        struct directory *dir;
        struct directory_entry ent;
        dir = directory_open(0);
        while (directory_get(dir, &ent) != -1){
            if(strcmp(ent.name, path + 1) == 0){
                return iget(ent.inode_num);
            }
        }

    }
    return NULL;
}

// basic version of mkdir
// args : char* -> the path to the new file
// returns : int, success status

// I feel like this came out more complicated than it needed to be
int directory_make(char *path){
    char dir[128];
    char base[128];
    struct inode *temp = namei(path);
    if(temp != NULL){
        perror("directory_make : directory exists");
        iput(temp);
        return -1;
    }

    get_dirname(path, dir);
    get_basename(path, base);

    struct inode *parent = namei(dir);
    if (parent == NULL) {
        perror("directory_make : namei()");
        iput(parent);
        return -1; // parent inode DNE
    }

    struct inode* new_dir = ialloc();
    int new_block_num = alloc();
    int new_inode_num = new_dir->inode_num;
    if(new_inode_num == -1){
        perror("directory_make : ialloc()");
        iput(parent);
        return -1;
    }
    if(new_block_num == -1){
        perror("directory_make : alloc()");
        iput(parent);
        return -1;
    }


    new_dir->flags = 2;
    new_dir->size = 2 * RECORD_SIZE;
    new_dir->block_ptr[0] = new_block_num;


    unsigned char block[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        block[i] = 0;
    }



    //initialize .
    int offset1 = 0;
    write_u16(block + offset1, new_dir->inode_num);
    block[offset1 + 2] = '.';
    block[offset1 + 3] = '\0';

    //initialize ..
    int offset2 = RECORD_SIZE;
    write_u16(block + offset2, parent->inode_num);
    block[offset2 + 2] = '.';
    block[offset2 + 3] = '.';
    block[offset2 + 4] = '\0';

    bwrite(new_block_num, block);

    //initialize the new directory in the parent  
    int parent_offset = parent->size;
    unsigned char parent_block[BLOCK_SIZE];
    bread(parent->block_ptr[0], parent_block);

    write_u16(parent_block + parent_offset, new_inode_num);

    //copy the name into the parent block
    int i = 0;
    while(base[i] != '\0'){
        parent_block[parent_offset + i + 2] = base[i];
        i++;
    }
    parent_block[parent_offset + 2 + i] = '\0';

    bwrite(parent->block_ptr[0], parent_block);

    parent->size += RECORD_SIZE;

    iput(new_dir);
    iput(parent);

    return 0;

}