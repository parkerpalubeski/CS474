TARGETS=split

.PHONY: all clean

all: $(TARGETS)

split: split.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)