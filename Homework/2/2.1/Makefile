TARGETS=ticketlock

.PHONY: all clean

all: $(TARGETS)

ticketlock: ticketlock.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)