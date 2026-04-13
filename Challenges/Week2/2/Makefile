TARGETS=cslock

.PHONY: all clean

all: $(TARGETS)

cslock: cslock.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)