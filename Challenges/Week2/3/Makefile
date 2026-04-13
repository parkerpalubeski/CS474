TARGETS=stack

.PHONY: all clean

all: $(TARGETS)

stack: stack.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)