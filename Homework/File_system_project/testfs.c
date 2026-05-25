//This is the central file that holds the main function (located at the bottom)
//This file also holds definitions for tests of the various functions present in other files

#include "ctest.h"
#include "inode.h" //only includes this because it has all dependencies
#include <string.h>

void test_inode(void){
    incore_free_all();
    struct inode *node1 = ialloc();
    CTEST_ASSERT(node1 != NULL, "testint ialloc returns a valid pointer");
    CTEST_ASSERT(node1->size == 0, "testing ialloc initialize value");
    struct inode *node2 = ialloc();
    CTEST_ASSERT(node2 != NULL, "testing ialloc succeeds when inodes exist");
    CTEST_ASSERT(node2 != node1, "testing two ialloc'd nodes are distinct pointers");
    struct inode *free = incore_find_free();
    CTEST_ASSERT(free != NULL, "testing incore_find_free() returns valid pointer");
    CTEST_ASSERT(incore_find(0) != NULL, "testing incore_find() returns pointer on inode_num in incore");
    CTEST_ASSERT(iget(0)!=NULL, "testing iget() returns pointer on inode in incore");
    CTEST_ASSERT(incore_find(6) == NULL, "testing incore_find() returns NULL on inode_num not in incore");
    incore_free_all();
    struct inode *nodes[MAX_SYS_OPEN_FILES];
    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++){
        nodes[i] = ialloc();
    }
    (void)nodes;
    struct inode *test_full = ialloc();
    CTEST_ASSERT(test_full == NULL, "testing ialloc returns null on full");
    incore_free_all();
    struct inode *node = iget(3);
    node->size = 12;
    iput(node);
    CTEST_ASSERT(node->ref_count == 0, "testing iput brings down ref_count to 0");
    return;
}



void test_block(void){
    unsigned char test_blockA[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        test_blockA[BLOCK_SIZE] = 'A';
    }
    bwrite(12, test_blockA); //changed to 12 : previous was 4 but 4 is an inode block
    unsigned char test_blockB[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        test_blockB[BLOCK_SIZE] = 'B';
    }
    //tests that bread() returns the same value as the value passed in argument 2
    CTEST_ASSERT((bread(12, test_blockB), test_blockB, BLOCK_SIZE), "testing bread() case : standard");
    //Tests that the contents of test_blockA and test_blockB are identical
    CTEST_ASSERT(memcmp(test_blockA, test_blockB, BLOCK_SIZE), "testing bread() stored values");
    //Tests that bread() returns NULL when passed a negative block_number
    CTEST_ASSERT(bread(-1, test_blockB) == NULL, "testing bread() case : negative block number");

    //Tests for alloc()
    CTEST_ASSERT(alloc() == 0, "testing alloc() on empty inode map");    
    CTEST_ASSERT(alloc() == 1, "Testing alloc() after first space allocated");

    return;

}

void test_img(void){
    int test_fd = image_open("test.img", 1);

    //tests that file descriptor is valid
    CTEST_ASSERT(test_fd != -1, "testing test_fd"); 

    //tests that test_fd is the same file descriptor as the global fd
    CTEST_ASSERT(test_fd == image_fd, "testing image_fd match to test_fd"); 

    //block.c function tests (function above)
    test_block(); 

    //inode.c function tests
    test_inode();

    //tests that image file closes successfully
    CTEST_ASSERT(image_close() == 0, "testing image_close"); 
    test_fd = image_open("Does_not_exist.img", 0);

    //Tests edge case: when truncate == 0 and yet file doesn't exist
    //Note:will fail if for some reason the file DOES exist
    CTEST_ASSERT(test_fd == -1, "testing test_fd when file does not exist"); 

    image_close();
    return;
}

void test_free(void){
    unsigned char test[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        test[i] = 0;
    }

    CTEST_ASSERT(find_free(test) == 0, "testing find_free() in empty test block");
    set_free(test, 0, 1);
    set_free(test, 2, 1);
    CTEST_ASSERT(find_free(test) == 1, "testing find_free() when 0 and 2 are marked");
    set_free(test, 1, 1);
    CTEST_ASSERT(find_free(test) == 3, "testing find_free() when 0-2 are marked");
    set_free(test, 0, 0);
    CTEST_ASSERT(find_free(test) == 0, "testing find_free() when first is free, 1 & 2 are not");
    for(int i = 0; i < BLOCK_SIZE; i++){
        test[i] = 255;
    }
    CTEST_ASSERT(find_free(test) == -1, "testing find_free() on full buffer");
    return;
}

int main(void){
    CTEST_VERBOSE(1);

    test_img();
    test_free();

    CTEST_RESULTS();
    CTEST_EXIT();

    return 0;
}