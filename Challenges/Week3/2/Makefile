TARGETS=mmap

.PHONY: all clean

all: $(TARGETS)

mmap: mmap.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)