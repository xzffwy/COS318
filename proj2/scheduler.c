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

    // pop new pcb off ready queue
    current_running = queue_pop(ready);

    // if ret of pop is null, then all tasks have exited so just loop forever
    while (!current_running){ ; }

    current_running->state = PROCESS_RUNNING;

    // return to scheduler_entry
}

void do_yield(void)
{
	// set pcb state
	current_running->state = PROCESS_READY;

	// call save_pcb, which should find the EIP from two calls up
	save_pcb();

	// push the currently running process on ready queue
	queue_push(ready, current_running);

	// call scheduler_entry
	scheduler_entry();

	// shouldn't get here i don't think
	ASSERT(0);
}

void do_exit(void)
{
	scheduler_entry();
}

void block(void)
{
	current_running->state = PROCESS_BLOCKED;

	save_pcb();

	queue_push(blocked, current_running);

	scheduler_entry();

	// shouldn't get here i don't think
	ASSERT(0);
}

void unblock(void)
{
	pcb_t *unblocked = queue_pop(blocked);
	if (!unblocked)
		return;

	unblocked->state = PROCESS_READY;
	queue_push(ready, unblocked);
}

bool_t blocked_tasks(void)
{
	return !blocked->isEmpty;
}
