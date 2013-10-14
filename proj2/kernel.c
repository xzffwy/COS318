/* kernel.c */

#include "common.h"
#include "kernel.h"
#include "scheduler.h"
#include "th.h"
#include "util.h" //DEBUG
#include "queue.h"

#include "tasks.c"

pcb_t *current_running;
queue_t ready_queue;
queue_t blocked_queue;
pcb_t *ready_arr[NUM_TASKS];
pcb_t *blocked_arr[NUM_TASKS];

/* This function is the entry point for the kernel
 * It must be the first function in the file
 */
void _start(void)
{
    /* Set up the single entry-point for system calls */
    *ENTRY_POINT = &kernel_entry;

    struct queue ready_q;
    ready_queue = &ready_q;
    ready_queue->pcbs = ready_arr;
    ready_queue->capacity = NUM_TASKS;
    queue_init(ready_queue);

    struct queue blocked_q;
    blocked_queue = &blocked_q;
    blocked_queue->pcbs = blocked_arr;
    blocked_queue->capacity = NUM_TASKS;
    queue_init(blocked_queue);

    clear_screen(0, 0, 80, 25);

    /* Initialize the pcbs and the ready queue */
    int iProcessIndex;
    int iStackTop = STACK_MIN;
    static pcb_t pcbs[NUM_TASKS];
    pcb_t *process = &pcbs[0];

    for (iProcessIndex = 0; iProcessIndex < NUM_TASKS; iProcessIndex++) {
    	struct task_info *thisTask = task[iProcessIndex];
        // populate instance variables
        iStackTop += STACK_SIZE;
    	process->esp = iStackTop;
    	process->ebp = iStackTop;
    	process->state = PROCESS_READY;
    	process->eip = thisTask->entry_point;
        process->eax = 0;
        process->ebx = 0;
        process->ecx = 0;
        process->edx = 0;
        process->edi = 0;
        process->esi = 0;
        process->eflags = 0;
        process->isKernel = (thisTask->task_type == KERNEL_THREAD) ? TRUE : FALSE;
        // add to queue
    	queue_push(ready_queue, process);

    	process++;
    }

    /* Schedule the first task */
    scheduler_count = 0;
    scheduler_entry();

    /* We shouldn't ever get here */
    ASSERT(0);
}



