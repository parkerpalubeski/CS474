#include <stdio.h>
#include <stdlib.h>

int glob;

int main(void)
{
    int var;
    int *heap = (int*)malloc(sizeof(int));
    printf("Stack location: %p\n", (void*)&var);
    printf("Heap location: %p\n", (void*)heap);
    printf("Global variable location: %p\n", (void*)&glob); 
    free(heap);
    return 0;
}

//The addressses are not always the same
//Apparently, this is a security feature called ASLR
//ASLR randomizes the location of the stack and the heap somewhat to prevent certain kinds of exploits, like buffer overflow