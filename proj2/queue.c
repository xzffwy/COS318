/* queue.c */

#include "common.h"
#include "scheduler.h"
#include "th.h"
#include "util.h"

/* Push the pcb for a task onto the queue. Return true if successful, or false if full */
bool_t push(queue_t queue, pcb_t pcb) {
	// If the queue is full, return false
	if (!queue->isEmpty && (queue->head == queue->tail)) {
		return FALSE;
	}

	// push the pcb
	queue->pcbs[queue->tail] = pcb;
	// set new tail
	queue->tail++;
	queue->tail %= QUEUE_SIZE;

	return TRUE;
}

/* Pop a pcb for a task off of the queue. */
pcb_t pop(queue_t queue) {
	// If the queue is empty, return null
	if (queue->isEmpty) {
		return null;
	}

	pcb_t oPopped = queue->pcbs[queue->head];

	// set new head
	queue->head++;
	queue->head %= QUEUE_SIZE;
	// track whether queue was just made empty
	if (queue->head == queue->tail) {
		queue->isEmpty = TRUE;
	}

	return oPopped;
}

/* Initialize the queue */
void queue_init(queue_t queue) {
	queue->head = 0;
	queue->tail = 0;
	queue->isEmpty = TRUE;
}

int main() {
	queue_t queue;
	queue = malloc(sizeof(queue_t));

	return 0;
}
