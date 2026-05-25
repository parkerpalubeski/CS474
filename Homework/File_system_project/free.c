#include "free.h"

void set_free(unsigned char *block, int num, int set){
    int byte_num = num / 8;
    int bit_num = num % 8;
    if (set == 1) {
        block[byte_num] |= (1 << bit_num);
    } else {
        block[byte_num] &= ~(1 << bit_num);
    }
}

int find_free(unsigned char *block){
    for (int byte_num = 0; byte_num < BLOCK_SIZE; byte_num++) {
        //based on find_low_clear_bit() in the instructions
        for(int i = 0; i < 8; i++){
            int check = block[byte_num] & (1 << i);
            if(check == 0){
                return (byte_num * 8) + i;
            }
        }
    }
    return -1;
}