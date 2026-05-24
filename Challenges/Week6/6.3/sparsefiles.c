#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd = open("foo.txt", O_CREAT|O_TRUNC|O_WRONLY, 0600);
    lseek(fd, 400000000, SEEK_SET);
    write(fd, "A", 1);
    close(fd);
}

//The file is 382 megabytes. Holy.

//The total disk consumption is 32 kilobytes (assuming no source files or executable, the file alone is only about 4 kilobytes)

//This is because it is technically a "sparse file". The file system optimizes those files that have big "holes" in them, and the gaps are recorded as simple metadata rather than being considered equivalent to occupied spaces.