#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms
#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

int clock = 0;

/**
 * Process information.
 */
struct process {
    int pid;
    int time_awake_remaining;
};

/**
 * The process table.
 */
struct process table[MAX_PROCS];

/**
 * Initialize the process table.
 */
void init_proc_table(void)
{
    for (int i = 0; i < MAX_PROCS; i++) {
        table[i].pid = i;
        table[i].time_awake_remaining = 0;
    }
}

/**
 * Parse the command line.
 */
void parse_command_line(int argc, char **argv)
{
    for(int i = 0; i < argc - 1; i++) {
        table[i].pid = i;
        table[i].time_awake_remaining = atoi(argv[i + 1]);
    }
}

/**
 * Main.
 */
int main(int argc, char **argv)
{
    struct queue *q = queue_new();

    init_proc_table();

    parse_command_line(argc, argv);

    // TODO
    for(int i = 0; i < argc - 1; i++) {
        queue_enqueue(q, &table[i]);
    }

    while(queue_is_empty(q) != 1) {
        int time_slice = 40;
        struct process *p = queue_dequeue(q);
        if(p->time_awake_remaining < time_slice) {
            time_slice = p->time_awake_remaining;
        }
        printf("=== Clock %d ms ===\n", clock);
        printf("PID %d: Running\n", p->pid);
        p->time_awake_remaining -= time_slice;
        printf("PID %d: Ran for %d ms\n", p->pid, time_slice);

        if(p->time_awake_remaining > 0){queue_enqueue(q, p);}

        clock += time_slice;
    }

    queue_free(q);
}
