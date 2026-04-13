TARGETS=tslock

.PHONY: all clean

all: $(TARGETS)

tslock: tslock.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)