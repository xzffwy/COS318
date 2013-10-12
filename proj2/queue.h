/* queue.h: definitions of functions in queue code */

#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include "tasks.h"

#define QUEUE_SIZE NUM_TASKS

typedef struct {
	pcb_t pcbs[NUM_TASKS];
	uint32_t head;
	uint32_t tail;
	bool_t isEmpty;
} queue_t;

/* Push the pcb for a task onto the queue. Return true if successful, or false if full */
bool_t push(queue_t queue, pcb_t pcb);

/* Pop a pcb for a task off of the queue. */
pcb_t pop(queue_t queue);

/* Initialize the queue */
void queue_init(queue_t queue);

#endif                          /* QUEUE_H */
