TARGETS=grow

.PHONY: all clean

all: $(TARGETS)

grow: grow.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)