/* kernel.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "th.h"
#include "util.h"
#include "queue.h"

#include "tasks.c"

pcb_t *current_running;

/* This function is the entry point for the kernel
 * It must be the first function in the file
 */
void _start(void)
{
    /* Set up the single entry-point for system calls */
    *ENTRY_POINT = &kernel_entry;

    // declare these here in order for memory to be statically allocated since we dont have malloc
    queue_t ready;
    queue_t blocked;

    queue_init(ready);
    queue_init(blocked);

    clear_screen(0, 0, 80, 25);

    /* Initialize the pcbs and the ready queue */
    int iProcessIndex;
    int iStackTop = STACK_MIN;
    pcb_t pcbs[NUM_TASKS];
    for (iProcessIndex = 0; iProcessIndex < NUM_TASKS; iProcessIndex++) {
    	iStackTop += STACK_SIZE;

    	// TO-DO: push onto ready queue
    	pcbs[iProcessIndex]->esp = iStackTop;
    	pcbs[iProcessIndex]->ebp = iStackTop;
    	pcbs[iProcessIndex]->state = PROCESS_READY;
    	pop(ready);
    }

    /* Schedule the first task */
    scheduler_count = 0;
    scheduler_entry();

    /* We shouldn't ever get here */
    ASSERT(0);
}



