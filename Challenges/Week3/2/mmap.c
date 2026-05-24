#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

const int SIZE=1024;  // bytes

struct bicycle {
    int wheel_count;
    char name[32];
};

int main(void)
{
    // Allocate some space from the OS
    void *heap = mmap(NULL, SIZE, PROT_READ|PROT_WRITE,
                  MAP_ANON|MAP_PRIVATE, -1, 0);

    // Challenge 1:
    //
    // Treat heap as an array of chars
    // Store a string there with strcpy() and print it
    char *str = "Hello, world!";
    strcpy((char *)heap, str);
    printf("%s\n", (char *)heap);

    // Challenge 2:
    //
    // Treat heap as an array of ints
    // Use a loop to store the values 0, 10, 20, 30, 40 in it
    // Use a loop to retrieve the value and print them
    int *int_array = (int *)heap;
    for (int i = 0; i < 5; i++) {
        int_array[i] = i * 10;
    }
    for (int i = 0; i < 5; i++) {
        printf("%d\n", int_array[i]);
    }

    // Challenge 3:
    //
    // Treat heap as an array of struct bicycles
    // Store 3 bicycles
    // Print out the bicycle data in a loop
    struct bicycle *bikes = (struct bicycle*)heap;
    for (int i = 0; i < 3; i++) {
        bikes[i].wheel_count = 2;
    }
    strcpy(bikes[0].name, "Mountain bike");
    strcpy(bikes[1].name, "Road bike");
    strcpy(bikes[2].name, "Eletric bike");
    for (int i = 0; i < 3; i++) {
        printf("Bike %d: %s with %d wheels\n", i, bikes[i].name, bikes[i].wheel_count);
    }

    // Challenge 4:
    //
    // If you used pointer notation, redo challenges 1-3 with array
    // notation, and vice versa.
    struct bicycle *bikes2 = (struct bicycle*)heap;
    for (int i = 0; i < 3; i++) {
        bikes2[i].wheel_count = 2;
    }
    strcpy(bikes2[0].name, "Mountain bike");
    strcpy(bikes2[1].name, "Road bike");
    strcpy(bikes2[2].name, "Eletric bike");
    for (int i = 0; i < 3; i++) {
        printf("Bike %d: %s with %d wheels\n", i, (bikes2 + i)->name, (bikes2 + i)->wheel_count);
    }


     

    // Challenge 5:
    //
    // Make the first 32 bytes of the heap a string (array of chars),
    // and follow that directly with an array of ints.
    //
    // Store a short string in the string area of the heap
    // Use a loop to store the values 0, 10, 20, 30, 40 in the int array
    // Use a loop to retrieve the value and print them
    // Print the string
    char *str2 = "A string";
    strcpy((char *)heap, str2);
    int *int_array2 = (int *)(heap + 32);
    for (int i = 0; i < 5; i++) {
        int_array2[i] = i * 10;
    }
    for (int i = 0; i < 5; i++) {
        printf("%d\n", *(int*)(heap + 32 + (i * sizeof(int))));
    }
    printf("String: %s\n", (char *)heap);

    // Challenge 6:
    //
    // Make an array of interleaved `struct bicycle` and `int` data
    // types.
    //
    // The first entry in the heap will be a `struct bicycle`, then
    // right after that, an `int`, then right after that a `struct
    // bicycle`, then an `int`, etc.
    //
    // Store as many of these as can fit in SIZE bytes. Loop through,
    // filling them up with programmatically-generated data. Then loop
    // through again and print out the elements.
    for(int i = 0; i < SIZE; i++) {
        if(i + sizeof(struct bicycle) < (unsigned long)SIZE) {
            struct bicycle *bike = (struct bicycle *)(heap + i);
            //This will have some absurd wheel counts but just to discern between them since the names are the same
            bike->wheel_count = 2 + (i / sizeof(struct bicycle)); 
            strcpy(bike->name, "Ex. Bike");
            i += sizeof(struct bicycle);
        }
        if(i + sizeof(int) < (unsigned long)SIZE) {
            int *num = (int *)(heap + i);
            *num = (i / sizeof(int) * 2000);
            i += sizeof(int);
        }
    }
    for(int i = 0; i < SIZE; i++) {
        if(i + sizeof(struct bicycle) < (unsigned long)SIZE) {
            struct bicycle *bike = (struct bicycle *)(heap + i);
            printf("%s: %d wheels\n", bike->name, bike->wheel_count);
            i += sizeof(struct bicycle);
        }
        if(i + sizeof(int) < (unsigned long)SIZE) {
            int *num = (int *)(heap + i);
            printf("Integer: %d\n", *num);
            i += sizeof(int);
        }
    }

    // Free it up
    munmap(heap, SIZE);
}