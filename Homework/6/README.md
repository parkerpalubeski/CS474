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

## Files

* `testfs.c`: The central file to launch the program and tests
* `block.c`: Provides functions for reading and writing to the "blocks", as well as calculating offsets (in other words, WHERE to write to the file)
* `image.c`: Includes functions that open and close the file and provides file pointers
* `ctest.h`: Header file provided by Beej that includes tests to assure the program is working properly


## Data

This program uses arrays of chars referred to as "blocks" to simulate blocks of memory, since chars take up one byte of memory exactly. In effect, these are only used in their binary forms.

## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`: main function that calls other functions
  * `CTEST_VERBOSE(): Selects whether to print individual test results
  * `test_img()`: Tests file opening/closing and calls function to test block.c
    * `image_open()`: Opens a file and returns a file pointer
    * `CTEST_ASSERT()`: Tests whether a condition is true
    * `test_block()`: Tests functions in block.c
    * `bwrite()`: Writes to the open file, calculating offset based on provided block number
    * `image_close()`: Closes a file pointer
  * `CTEST_RESULTS()`: Prints out percentage of tests passed
  * `CTEST_EXIT()`: Ends tests