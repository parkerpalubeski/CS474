#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROG_LEN 10 // Max terms in a "program"
#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms

#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

struct process 
 {
    enum status { READY, ASLEEP, EXITED } status;
    int pid;
    int sleep_time_remaining;
    int run_time_remaining;
    int pc;
    int instr[MAX_PROG_LEN];
};

struct process table[MAX_PROCS];


void parse_cl(int argc, char** argv)
{
    for(int i = 0; i < argc - 1; i++)
    {
        table[i].pid = i;
        table[i].status = READY;
        table[i].pc = 0;
        char* tok = strtok(argv[i+1], ", \n");
        table[i].run_time_remaining += atoi(tok);
        int j = 1;
        while((tok = strtok(NULL, ", \n"))!= NULL)
        {
            table[i].instr[j]=atoi(tok);
            j++;
        }
        table[i].instr[j] = 0;
    }
}

//Runs if queue is empty
//Returns time until next program wakes
int next_process(int argc)
{
    //Find the program that will wake up next/time to wake
    int t = 999999;
    for(int i = 0; i < argc - 1; i++)
    {
        if(table[i].status == ASLEEP && table[i].sleep_time_remaining < t)
        {
            t = table[i].sleep_time_remaining;
        }
    }
    //Subtract time to wake from all process sleep times
    for(int i = 0; i < argc - 1; i++)
    {
        table[i].sleep_time_remaining -= t;
    }
    return t;
}

void wake_processes(int argc, struct queue* q)
{
    for(int i = 0; i < argc - 1; i++)
    {
        if(table[i].status == ASLEEP && table[i].sleep_time_remaining <= 0)
        {
            table[i].pc++;
            if(table[i].instr[table[i].pc] == 0)
            {
                printf("PID %d: Exiting\n", table[i].pid);
                table[i].status = EXITED;
            }
            else
            {
                table[i].run_time_remaining = table[i].instr[table[i].pc];
                printf("PID %d: Waking up for %d ms\n", table[i].pid, table[i].run_time_remaining);
                table[i].status = READY;
                queue_enqueue(q, &table[i]);
            }
        }
    }
}

int run_process(struct process* p, struct queue* q, int argc)
{
    int t;
    if(p->run_time_remaining < QUANTUM){t = p->run_time_remaining;}
    else{t = QUANTUM;}
    p->run_time_remaining -= t;
    for(int i = 0; i < argc - 1; i++)
    {
        if(table[i].status == ASLEEP){table[i].sleep_time_remaining -= t;}
    }
    if(p->run_time_remaining > 0){queue_enqueue(q, p);}
    else
    {
        p->pc += 1;
        if(p->instr[p->pc] == 0)
        {
            printf("PID %d: Exiting\n", p->pid);
            p->status = EXITED;
        }
        else
        {
            p->status = ASLEEP;
            p->sleep_time_remaining = p->instr[p->pc];
            printf("PID %d: Sleeping for %d ms\n", p->pid, p->sleep_time_remaining);
        }
    }
    return t;
}

/**
 * Main.
 */
int main(int argc, char **argv)
{
    int clock = 0;

    struct queue *q = queue_new();

    parse_cl(argc, argv);

    for(int i = 0; i < argc - 1; i++) {queue_enqueue(q, &table[i]);}



    int complete = 0;
    struct process* p;
    while(1)
    {
        //sleep(1);
        //Check for non-exited processes
        for(int i = 0; i < argc - 1; i++)
        {
            complete = 1;
            if(table[i].status != EXITED) {complete = 0; break;}
            else {continue;}
        }
        if(complete){break;}


        //If run queue empty
        if(queue_is_empty(q))
        {
            clock += next_process(argc);
        }

        //Print clock value
        printf("=== Clock %d ms ===\n", clock);
        //Wake processes
        wake_processes(argc, q);
        //Cycle runner
        if(!queue_is_empty(q)){
            p = queue_dequeue(q);
        }
        else
        {
            continue;
        }
        //Print out PID is running
        printf("PID %d: Running\n", p->pid);
        //Run the process
        int t = run_process(p, q, argc);
        clock += t;
        printf("PID %d: Ran for %d ms\n", p->pid, t);
    }
}
