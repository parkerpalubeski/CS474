#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

const int SIZE=1024;  // bytes

struct firework {
    int shell_caliber;
    float lift_charge;
    float burst_charge;
    int fuse_delay_ms;
};

int main(void)
{
    // Allocate some space from the OS
    void *heap = mmap(NULL, SIZE, PROT_READ|PROT_WRITE,
                      MAP_ANON|MAP_PRIVATE, -1, 0);

    // Challenge: Store your favorite letter at the byte `heap` points
    // to, then print it with `%c` and also with `%d`.
    // TODO
    *(char *)heap = 'A';
    printf("%c\n", *(char *)heap);
    printf("%d\n", *(char *)heap);

    // Challenge: Store a string at the byte `heap` points to, then
    // print it.
    // TODO
    char *str = "Example string";
    strcpy((char *)heap, str);
    printf("%s\n", (char *)heap);

    // Challenge: Store an integer at the byte `heap` points to, then
    // print it.
    // TODO
    *(int *)heap = 42;
    printf("%d\n", *(int *)heap);

    // Challenge: Store a float right *after* the int you just stored.
    // Print them both.
    // TODO
    *(float *)(heap + sizeof(int)) = 2.22;
    printf("%d\n", *(int *)heap);
    printf("%f\n", *(float *)(heap + sizeof(int)));

    // Challenge: Store a short (a smallish integer) at byte offset 512
    // in the `heap` area. Then print it with `%d`, along with the int
    // and float you already stored.
    // TODO
    *(short *)(heap + 512) = 123;
    printf("%d\n", *(int *)heap);
    printf("%f\n", *(float *)(heap + sizeof(int)));
    printf("%d\n", *(short *)(heap + 512));

    // Challenge: Store a struct firework at byte offset 48 in the
    // `heap` area. Make a `struct firework *` initialized to the right
    // position, then fill it with data (with the `->` operator). Then
    // print out the data, and also print out the int, float, and short
    // from above.
    // TODO
    struct firework *fw = (struct firework *)(heap + 48);
    fw->shell_caliber = 22;
    fw->lift_charge = 1.0;
    fw->burst_charge = 2.0;
    fw->fuse_delay_ms = 2000;
    printf("%d\n", fw->shell_caliber);
    printf("%f\n", fw->lift_charge);
    printf("%f\n", fw->burst_charge);
    printf("%d\n", fw->fuse_delay_ms);
    printf("%d\n", *(int *)heap);
    printf("%f\n", *(float *)(heap + sizeof(int)));
    printf("%d\n", *(short *)(heap + 512));

    // Challenge: Store a second struct firework directly after the
    // first one. Initialize and print.
    // TODO
    struct firework *fw2 = (struct firework *)(heap + 48 + sizeof(struct firework));
    fw2->shell_caliber = 400;
    fw2->lift_charge = 10.0;
    fw2->burst_charge = 6.0;
    fw2->fuse_delay_ms = 1500;
    printf("%d\n", fw2->shell_caliber);
    printf("%f\n", fw2->lift_charge);
    printf("%f\n", fw2->burst_charge);
    printf("%d\n", fw2->fuse_delay_ms);
}