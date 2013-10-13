/* kernel.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "th.h"
#include "util.h"
#include "queue.h"

#include "tasks.c"

pcb_t *current_running;
queue_t ready_queue;
queue_t blocked_queue;

/* This function is the entry point for the kernel
 * It must be the first function in the file
 */
void _start(void)
{
    /* Set up the single entry-point for system calls */
    *ENTRY_POINT = &kernel_entry;

    struct queue ready_q;
    struct queue blocked_q;
    ready_queue = &ready_q;
    blocked_queue = &blocked_q;
    queue_init(ready_queue);
    queue_init(blocked_queue);

    clear_screen(0, 0, 80, 25);

    /* Initialize the pcbs and the ready queue */
    int iProcessIndex;
    int iStackTop = STACK_MIN;
    pcb_t pcbs[NUM_TASKS];
    pcb_t *process = &pcbs[0];
    for (iProcessIndex = 0; iProcessIndex < NUM_TASKS; iProcessIndex++) {
    	iStackTop += STACK_SIZE;
    	process++;

    	struct task_info *thisTask = task[iProcessIndex];

    	process->esp = iStackTop;
    	process->ebp = iStackTop;
    	process->state = PROCESS_READY;
    	process->eip = thisTask->entry_point;
    	// TO-DO: maybe consider thread type?
    	queue_push(ready_queue, process);
    }

    /* Schedule the first task */
    scheduler_count = 0;
    scheduler_entry();

    /* We shouldn't ever get here */
    ASSERT(0);
}



