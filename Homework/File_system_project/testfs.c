#include "free.h"
#include "ctest.h"
#include "image.h"
#include <string.h>

void test_block(){
    unsigned char test_blockA[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        test_blockA[BLOCK_SIZE] = 'A';
    }
    bwrite(4, test_blockA);
    unsigned char test_blockB[BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++){
        test_blockB[BLOCK_SIZE] = 'B';
    }
    //tests that bread() returns the same value as the value passed in argument 2
    CTEST_ASSERT((bread(4, test_blockB), test_blockB, BLOCK_SIZE), "testing bread() case : standard");
    //Tests that the contents of test_blockA and test_blockB are identical
    CTEST_ASSERT(memcmp(test_blockA, test_blockB, BLOCK_SIZE), "testing bread() stored values");
    //Tests that bread() returns NULL when passed a negative block_number
    CTEST_ASSERT(bread(-1, test_blockB) == NULL, "testing bread() case : negative block number");

    return;

}

void test_img(){
    int test_fd = image_open("test.img", 1);
    //tests that file descriptor is valid
    CTEST_ASSERT(test_fd != -1, "testing test_fd"); 
    //tests that test_fd is the same file descriptor as the global fd
    CTEST_ASSERT(test_fd == image_fd, "testing image_fd match to test_fd"); 
    //block.c function tests (function above)
    test_block(); 
    //tests that image file closes successfully
    CTEST_ASSERT(image_close() == 0, "testing image_close"); 
    test_fd = image_open("Does_not_exist.img", 0);

    //Tests edge case: when truncate == 0 and yet file doesn't exist
    //Note:will fail if for some reason the file DOES exist
    CTEST_ASSERT(test_fd == -1, "testing test_fd when file does not exist"); 
    image_close();
    return;
}

int main(){
    CTEST_VERBOSE(1);

    test_img();

    CTEST_RESULTS();
    CTEST_EXIT();

    return 0;
}