#include <stdio.h>

#define BITS_PER_BYTE 8

unsigned char bitmap[4096];

void print_byte(unsigned char b)
{
    if (b <= 1)
        printf("%d", b);

    else {
        print_byte(b >> 1);
        printf("%d", b & 1);
    }
}

void print_byte_n(unsigned char b)
{
    print_byte(b);
    putchar('\n');
}

int get_bit(int index)
{
    // TODO
    int byte = index / BITS_PER_BYTE;
    int bit_offset = index % BITS_PER_BYTE;

    return bitmap[byte] >> bit_offset & 1;
}

void set_bit(int index, int value)
{
    // TODO
    if(index < 0 || index > 32767){perror("set_bit index");};
    if(value != 0 && value != 1){perror("set_bit value");};

    int byte = index / BITS_PER_BYTE;
    int bit_offset = index % BITS_PER_BYTE;

    bitmap[byte] |= (1 << bit_offset);

    return;
}   

int main(void)
{
    set_bit(0,1);
    set_bit(2,1);
    set_bit(3,1);
    set_bit(4,1);
    set_bit(7,1);
    set_bit(8,1);

    print_byte_n(bitmap[0]);
    print_byte_n(bitmap[1]);

    for (int i = 0; i <= 9; i++) {
        printf("%d: %d\n", i, get_bit(i));
    }
}