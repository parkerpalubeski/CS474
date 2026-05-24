CC = gcc
CFLAGS = -Wall -Wextra -Werror

main: main.o adder.o
	$(CC) $(CFLAGS) -o main main.o adder.o

main.o: main.c main.h adder.h
	$(CC) $(CFLAGS) -c main.c

adder.o: adder.c adder.h
	$(CC) $(CFLAGS) -c adder.c

clean:
	rm -f *.o main