/* queue.h: definitions of functions in queue code */

#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 1 //TODO: define this dynamically!!!

typedef struct queue {
	pcb_t *pcbs[QUEUE_SIZE]; //array of pcb_t pointers
	uint32_t head;
	uint32_t tail;
	bool_t isEmpty;
} *queue_t;


/* Push the pcb for a task onto the queue. Return true if successful, or false if full */
bool_t queue_push(queue_t queue, pcb_t *pcb);

/* Pop a pcb for a task off of the queue. */
pcb_t* queue_pop(queue_t queue);

/* Initialize the queue */
void queue_init(queue_t queue);

/* Return the size of the queue */
uint32_t queue_size(queue_t queue);

#endif                          /* QUEUE_H */
