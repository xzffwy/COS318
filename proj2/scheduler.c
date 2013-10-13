/* scheduler.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "util.h"
#include "queue.h"

int scheduler_count;

void scheduler(void)
{
    ++scheduler_count;

    // pop new pcb off ready queue
    current_running = queue_pop(ready_queue);
    print_str(5, 0, "just popped a task with eip "); //DEBUG
    print_hex(6, 0, current_running->eip);

    // if ret of pop is null, then all tasks have exited so just loop forever
    while (!current_running){ 
    	print_str(0, 0, "What is my purpose in the absence of tasks?"); 
    }

    current_running->state = PROCESS_RUNNING;

    // return to scheduler_entry
}

void do_yield(void)
{
	// call save_pcb, which should find the EIP from two calls up
	save_pcb();

	// set pcb state
	current_running->state = PROCESS_READY;
	print_str(8, 0, "just saved a thread with eip: "); //DEBUG
    print_hex(8, 30, current_running->eip); //DEBUG
    while(1) {;} //DEBUG

	// push the currently running process on ready queue
	queue_push(ready_queue, current_running);

	// call scheduler_entry
	scheduler_entry();

	// should never reach here
	ASSERT(0);
}

void do_exit(void)
{
	current_running->state = PROCESS_EXITED;
	scheduler_entry();
}

void block(void)
{
	save_pcb();

	current_running->state = PROCESS_BLOCKED;

	queue_push(blocked_queue, current_running);

	scheduler_entry();

	// should never reach here
	ASSERT(0);
}

void unblock(void)
{
	pcb_t *unblocked = queue_pop(blocked_queue);
	if (!unblocked)
		return;

	unblocked->state = PROCESS_READY;
	queue_push(ready_queue, unblocked);
}

bool_t blocked_tasks(void)
{
	return !blocked_queue->isEmpty;
}
