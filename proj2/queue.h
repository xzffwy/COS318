/* queue.h: definitions of functions in queue code */

#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include "tasks.h"

#define QUEUE_SIZE NUM_TASKS

typedef struct {
	uint32_t pcbIDs[NUM_TASKS];
	uint32_t head;
	uint32_t tail;
	bool_t isEmpty;
} queue_t;

/* Push the pcb for a task onto the queue. Return true if successful, or false if full */
bool_t push(queue_t queue, uint32_t pcbID);

/* Pop a pcb for a task off of the queue. */
uint32_t pop(queue_t queue);

/* Initialize the queue */
void queue_init(queue_t queue);

#endif                          /* QUEUE_H */
