#include "ls.h"

// prints out the contents of a directory in format "%d %s"
// args : none
// returns : none
void ls(void)
{
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1)
        printf("%d %s\n", ent.inode_num, ent.name);

    directory_close(dir);
}