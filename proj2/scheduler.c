/* scheduler.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "util.h"

int scheduler_count;

/* Declare queues here */


void scheduler(void)
{
    ++scheduler_count;

    // pop new pcb off ready queue

    // if ret of pop is null, then all tasks have exited (TO-DO : can blocked queue ever not be empty here?) so just loop forever

    // return to scheduler_entry
}

void do_yield(void)
{
	// push the currently running process on ready queue

	// go-to scheduler_entry
}

void do_exit(void)
{
}

void block(void)
{
}

void unblock(void)
{
}

bool_t blocked_tasks(void)
{
}
