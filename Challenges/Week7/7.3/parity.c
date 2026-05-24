#include <stdio.h>

#define TEST_COUNT 16

int parity(unsigned char b)
{
    //Use addition
    int add = 0;
    unsigned char temp;
    temp = b;
    while(temp){
        add += temp & 1;
        temp = temp >> 1;
    }
    add %= 2;


    //XOR
    int parity_bit = 0;
    while(b){
        parity_bit = parity_bit ^ (b & 1);
        b = b >> 1;
    }

   //simple assert that both add and xor are equal 
    if(add == parity_bit){
        return parity_bit;
    }
    return -1;

}

int main(void)
{
    char *green = "\033[0;92m";  // Green
    char *red = "\033[0;30;101m";  // Black on red
    char *reset = "\033[0m";

    unsigned char test[TEST_COUNT] = {
        0x00, 0xff, 0x01, 0x80, 0xaa, 0x9a, 0x19, 0xaf,
        0x75, 0x2d, 0x87, 0x4f, 0x93, 0x49, 0xf4, 0x30
    };

    int results[TEST_COUNT] = {
        0, 0, 1, 1, 0, 0, 1, 0,
        1, 0, 0, 1, 0, 1, 1, 0
    };

    for (int i = 0; i < TEST_COUNT; i++) {
        int result = parity(test[i]);

        if (result == results[i])
            printf("%sPASS%s: %02x -> %d\n", green, reset, test[i], result);
        else
            printf("%sFAIL%s: %02x -> %d (expected %d)\n", red, reset, test[i],
                result, results[i]);
    }
}