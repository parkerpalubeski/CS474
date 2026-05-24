TARGETS=bitwise

.PHONY: all clean

all: $(TARGETS)

bitwise: bitwise.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)