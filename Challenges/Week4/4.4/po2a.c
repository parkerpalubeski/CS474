#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 256

int main(int argc,  char* argv[])
{
    if(argc != 3)
    {
        printf("usage: po2a int int\n");
        return 1;
    }
    unsigned long page = atoi(argv[1]);
    unsigned long offset = atoi(argv[2]);
    unsigned long addr = (page << 8) + offset;
    printf("Page %ld, offset %ld = Address %ld\n", page, offset, addr);
    
    return 0;
}