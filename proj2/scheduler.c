/* scheduler.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "util.h"
#include "queue.h"

int scheduler_count;

/* Declare queues here */
queue_t blocked;
queue_t ready;

void scheduler(void)
{
    ++scheduler_count;

    if (!blocked_tasks()) {
    	while(1);
    }

    // pop new pcb off ready queue

    // if ret of pop is null, then all tasks have exited (TO-DO : can blocked queue ever not be empty here? DONE: exit releases lock) so just loop forever

    // return to scheduler_entry
}

void do_yield(void)
{
	// push the currently running process on ready queue

	// go-to scheduler_entry
}

void do_exit(void)
{
	// release any locks
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
