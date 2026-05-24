#include <stdio.h>
#include <stdlib.h>

void grow_stack(int num)
{
    int* var = (int*)malloc(sizeof(int));
    printf("Stack variable #%d at address: %p\n", num, var);
    if(num < 5)
    {
        grow_stack(num + 1);
    }
    return;
}

int main(void)
{
    int *ptrs[5];
    for(int i = 0; i < 5; i++){
        ptrs[i] = (int*)malloc(sizeof(int));
        printf("Heap variable #%d at address: %p\n", i, ptrs[i]);
    }
    for(int i = 0; i < 5; i++){
        free(ptrs[i]);
    }
    grow_stack(0);
}
//OUTPUT:
//Heap variable #0 at address: 0x55ea3cc052a0
//Heap variable #1 at address: 0x55ea3cc056d0
//Heap variable #2 at address: 0x55ea3cc056f0
//Heap variable #3 at address: 0x55ea3cc05710
//Heap variable #4 at address: 0x55ea3cc05730
//Stack variable #0 at address: 0x55ea3cc05730
//Stack variable #1 at address: 0x55ea3cc05710
//Stack variable #2 at address: 0x55ea3cc056f0
//Stack variable #3 at address: 0x55ea3cc056d0
//Stack variable #4 at address: 0x55ea3cc052a0
//Stack variable #5 at address: 0x55ea3cc05750