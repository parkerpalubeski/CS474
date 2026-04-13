TARGETS=rwspinlock

.PHONY: all clean

all: $(TARGETS)

rwspinlock: rwspinlock.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)