#include "image.h"

int image_fd;

// opens a new file and assigns the image_fd to it
// note : does not do so if truncate is disabled and the file DNE
// args : char* int
// returns : int (fd)
int image_open(char *filename, int truncate)
{
    if (truncate == 1)
    {
        image_fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0600);
        return image_fd;
    }
    else
    {
        image_fd = open(filename, O_RDWR, 0600);
        return image_fd;
    }
}

// closes the global fd
// args : none
// returns : int (success/fail)
int image_close(void)
{
    return close(image_fd);
}