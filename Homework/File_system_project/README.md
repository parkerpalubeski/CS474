# Very Very Simple File System (VVSFS)

## Building

Command line:

* `make` to build. An executable called `testfs` will be produced
* `make test` to build and automatically run the program

VS Code:

* The default build task runs `make`.
* `make test` builds the program and then automatically runs it
* `make clean` to remove object files
* `make pristine` to remove all build products, .img files, and object files
* `make run` to run the program WITHOUT running tests (in other words, only the file system!)

## Files

* `testfs.c`: The central file to launch the program and tests
* `block.c` : Provides functions for reading and writing to the "blocks", as well as calculating offsets (in other words, WHERE to write to the file)
* `image.c` : Includes functions that open and close the file and provides file pointers
* `ctest.h` : Header file provided by Beej that includes tests to assure the program is working properly
* `free.c`  : Functions for finding and setting free memory blocks
* `inode.c` : Functions for handling inode support for our VVSFS
* `mkfs.c`  : Initializes the file system
* `pack.c*  : Helper functions for bitwise calculations on inodes
* `dir.c*   : Functions to open/close/read directories
* `ls.c*    : An ls()-like function (prints inode nums and filenames)


## Data

This program uses arrays of chars referred to as "blocks" to simulate blocks of memory, since chars take up one byte of memory exactly. In effect, these are only used in their binary forms. Blocks of memory are assigned different roles depending on their block number.

Each directory in the file system is represented byt a struct.

## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`: main function that calls other functions
  * `CTEST_VERBOSE(): Selects whether to print individual test results
  * `test_img()`: Tests file opening/closing and calls function to test block.c
    * `image_open()`: Opens a file and returns a file pointer
    * `CTEST_ASSERT()`: Tests whether a condition is true
    * `test_block()`: Tests functions in block.c
      * `bwrite()`: Writes to the open file, calculating offset based on provided block number
      * `bread()`: Reads from the opened file into a block of memory
      * `alloc()`: allocates memory in the BLOCK_MAP and writes to the opened file
    * `test_inode()`: Tests functions in inode.c
      * `incore_free_all()`: Sets the ref count of all inodes in the incore array to 0
      * `ialloc()`: allocates memory in the inode map and returns the a pointer to the lowest free inode
      * `incore_find()`: Finds an in-core inode record in the incore array
      * `iget()`: Returns a pointer for an in-core inode at inode_num
      * `iput()`: releases an active in-core inode and writes it back to disk if its ref_count drops to 0
    * `image_close()`: Closes a file pointer
  * `CTEST_RESULTS()`: Prints out percentage of tests passed
  * `CTEST_EXIT()`: Ends tests
  * `mkfs()`: Initializes the file system
  * `ls()`: Prints out all contents of the current directory
    * `directory_open()` : Takes an inode number and malloc's a directory struct, returning the pointer
    * `directory_get()` : reads data from a directory into a directory entry
    * `directory_close()` : frees the directory's inode and directory struct