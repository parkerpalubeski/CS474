#include "image.h"

int image_fd;

int image_open(char *filename, int truncate){
    if(truncate == 1){
        image_fd = open(filename, O_CREAT|O_TRUNC|O_RDWR, 0600);
        return image_fd;
    }
    else{
        image_fd = open(filename, O_RDWR, 0600);
        return image_fd;
    }
}

int image_close(void){
    return close(image_fd);
}