#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 256

int main(int argc, char* argv[])
{
    if(argc != 2) //error handle
    {
        printf("usage: a2po int\n");
        return 1;
    }
    unsigned long addr = atoi(argv[1]);
    unsigned long page = addr >> 8;
    unsigned long offset = addr & (~256);
    printf("Address %ld = Page %ld, offset %ld\n", addr, page, offset);
    return 0;
}