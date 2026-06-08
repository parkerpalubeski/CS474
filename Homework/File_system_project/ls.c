#include "ls.h"

// prints out the contents of a directory in format "%d %s"
// args : inode
// returns : none
void ls(int inode_num)
{
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(inode_num);

    while (directory_get(dir, &ent) != -1)
        printf("%d %s\n", ent.inode_num, ent.name);

    directory_close(dir);
}