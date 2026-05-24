TARGETS=data

.PHONY: all clean

all: $(TARGETS)

data: data.c
	gcc -Wall -Wextra -o $@ $^

clean:
	rm -f $(TARGETS)