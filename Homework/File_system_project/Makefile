CC = gcc
CFLAGS = -Wall -Wextra -Werror -DCTEST_ENABLE

all: testfs

# library
testfs: testfs.o libvvsfs.a
	$(CC) $(CFLAGS) testfs.o libvvsfs.a -o testfs

libvvsfs.a: image.o block.o free.o inode.o pack.o mkfs.o dir.o ls.o
	ar rcs libvvsfs.a image.o block.o free.o inode.o pack.o mkfs.o dir.o ls.o 

# makes and runs tests on make test, as well as the file system program
test: testfs
	./testfs test

# makes and then runs the file system program ONLY (no tests)
run: testfs
	./testfs

# make clean
clean:
	rm -f *.o testfs

# make pristine
pristine: clean
	rm -f testfs libvvsfs.a *.img