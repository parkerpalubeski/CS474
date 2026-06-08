// This is the central file that holds the main function (located at the bottom)
// This file also holds definitions for tests of the various functions present in other files

// TO DO:
// add more tests
// namei()
// directory_make()
// add deletion for test images

#include "ctest.h" //Beej testing functions
#include "mkfs.h"  //includes this because it has most dependencies

// Tests functions in inode.c
void test_inode(void)
{
    incore_free_all();
    struct inode *node1 = ialloc();
    CTEST_ASSERT(node1 != NULL, "testing ialloc returns a valid pointer");
    CTEST_ASSERT(node1->size == 0, "testing ialloc initialize value");
    struct inode *node2 = ialloc();
    CTEST_ASSERT(node2 != NULL, "testing ialloc succeeds when inodes exist");
    CTEST_ASSERT(node2 != node1, "testing two ialloc'd nodes are distinct pointers");
    struct inode *free = incore_find_free();
    CTEST_ASSERT(free != NULL, "testing incore_find_free() returns valid pointer");
    CTEST_ASSERT(incore_find(0) != NULL, "testing incore_find() returns pointer on inode_num in incore");
    CTEST_ASSERT(iget(0) != NULL, "testing iget() returns pointer on inode in incore");
    CTEST_ASSERT(incore_find(6) == NULL, "testing incore_find() returns NULL on inode_num not in incore");
    incore_free_all();
    struct inode *nodes[MAX_SYS_OPEN_FILES];
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
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

// Tests functions in block.c
void test_block(void)
{
    unsigned char test_blockA[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        test_blockA[BLOCK_SIZE] = 'A';
    }
    bwrite(12, test_blockA); // changed to 12 : previous was 4 but 4 is an inode block
    unsigned char test_blockB[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        test_blockB[BLOCK_SIZE] = 'B';
    }
    // tests that bread() returns the same value as the value passed in argument 2
    CTEST_ASSERT((bread(12, test_blockB), test_blockB, BLOCK_SIZE), "testing bread() case : standard");
    // Tests that the contents of test_blockA and test_blockB are identical
    CTEST_ASSERT(!memcmp(test_blockA, test_blockB, BLOCK_SIZE) == 0, "testing bread() stored values");
    // Tests that bread() returns NULL when passed a negative block_number
    CTEST_ASSERT(bread(-1, test_blockB) == NULL, "testing bread() case : negative block number");

    // Tests for alloc()
    CTEST_ASSERT(alloc() == 0, "testing alloc() on empty inode map");
    CTEST_ASSERT(alloc() == 1, "Testing alloc() after first space allocated");

    return;
}

// Tests functions in image.c
void test_img(void)
{
    int test_fd = image_open("test.img", 1);

    // tests that file descriptor is valid
    CTEST_ASSERT(test_fd != -1, "testing test_fd");

    // tests that test_fd is the same file descriptor as the global fd
    CTEST_ASSERT(test_fd == image_fd, "testing image_fd match to test_fd");

    // block.c function tests (function above)
    test_block();

    // inode.c function tests
    test_inode();

    // tests that image file closes successfully
    CTEST_ASSERT(image_close() == 0, "testing image_close");
    test_fd = image_open("Does_not_exist.img", 0);

    // Tests edge case: when truncate == 0 and yet file doesn't exist
    // Note:will fail if for some reason the file DOES exist
    CTEST_ASSERT(test_fd == -1, "testing test_fd when file does not exist");

    image_close();
    return;
}

// Tests all functions in free.c
void test_free(void)
{
    unsigned char test[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
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
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        test[i] = 255;
    }
    CTEST_ASSERT(find_free(test) == -1, "testing find_free() on full buffer");
    return;
}

// tests the dir functions
// creates a new image file after testing the other functions, then deletes it
void test_dir(void)
{
    int dir_fd = image_open("dir_test.img", 1);
    mkfs();
    incore_free_all();

    // test mkfs
    struct inode *root = iget(0);
    CTEST_ASSERT(root != NULL, "testing valid root node created with mkfs");
    CTEST_ASSERT(root->flags == 2, "testing root inode flag indicates that it's a directory");
    CTEST_ASSERT(root->size == 2 * RECORD_SIZE, "testing that 2 records are present in new root directory");

    // test directory functions
    struct directory *dir = directory_open(0);
    struct directory_entry ent;
    CTEST_ASSERT(dir != NULL, "testing directory_open doesn't return NULL");
    CTEST_ASSERT(directory_get(dir, &ent) == 0, "testing directory get returns 0 upon success");
    CTEST_ASSERT(strcmp(ent.name, ".") == 0, "testing 1st directory entry should be .");
    CTEST_ASSERT(directory_make("/foo") == 0, "testing directory_make creates valid directory foo");
    CTEST_ASSERT(directory_make("/bar") == 0, "testing directory_make creates valid directory bar");
    printf("ls of root directory\n");
    ls(0);
    printf("bar and foo should appear here\n"); 
    printf("ls of foo\n");
    ls(1);
    printf("ls of bar\n");
    ls(2);
    CTEST_ASSERT(directory_make("/foo") == -1, "testing directory_make fails when directory already exists");
    ls(0);
    directory_close(dir);

    (void)dir_fd;
    iput(root);
    image_close(); 
}

// Main function, calls all other functions
// Arguments are a personal liberty I took
// When entering "make test", "test" is passed as an argument into the function, starting testing mode and running all tests
// Otherwise, the filesystem runs as usual, or you can pass in the test argument manually with ./testfs test
// NOTE: tests write to "test.img" and "dir_test.img", while main function writes to "fs.img"
// args : "test" to initiate testing mode, or none
int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "test") == 0) // enter testing mode
        {
            // tests active
            CTEST_VERBOSE(1);

            // call tests
            test_img();
            test_free();
            test_dir();

            // prints ctest results
            CTEST_RESULTS();
            //runs the program again, this time not in testing mode
            main(1, NULL);
            CTEST_EXIT();
        }
        else // invalid args
        {
            printf("./testfs or ./testfs test\n");
            return 0;
        }
    }
    printf("===FILE SYSTEM RUN===\n");
    // mkfs call
    int fd = image_open("fs.img", 1);
    mkfs();
    (void)fd;
    ls(0);
    directory_make("/foo");
    ls(0);
    ls(1);
    image_close();

    // prevent errors with arguments
    (void)argc;
    (void)argv;

    return 0;
}