CC = gcc
CFLAGS = -Wall -Wextra -Werror -DCTEST_ENABLE

all: testfs

# library
testfs: testfs.o libvvsfs.a
	$(CC) $(CFLAGS) testfs.o libvvsfs.a -o testfs

libvvsfs.a: image.o block.o free.o inode.o pack.o
	ar rcs libvvsfs.a image.o block.o free.o inode.o pack.o

# run tests on make test
test: testfs
	./testfs

# make clean
clean:
	rm -f *.o testfs

# make pristine
pristine: clean
	rm -f testfs libvvsfs.a *.img