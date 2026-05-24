TARGETS=address

.PHONY: all clean

all: $(TARGETS)

address: address.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)