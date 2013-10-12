/* lock.c: mutual exclusion
 * If SPIN is false, lock_acquire() should block the calling process until its request can be satisfied
 * Your solution must satisfy the FIFO fairness property
 */

#include "common.h"
#include "lock.h"
#include "scheduler.h"

enum {
    SPIN = TRUE,
};

void lock_init(lock_t * l)
{
    if (SPIN) {
        l->status = UNLOCKED;
    } else {
    	l->status = UNLOCKED;
    }
}

void lock_acquire(lock_t * l)
{
    if (SPIN) {
        while (LOCKED == l->status)
            do_yield();
        l->status = LOCKED;
    } else {
        // if owner is -1 or status = UNBLOCKED -> lock_owner = running_task from kernel.c, status=BLOCKED, return
        // if owner is running_task, then wtf owner u crazy r sumtin?, i guess return
        // if neither of these are true, enqueue running task on blocked queue and call save_pcb, then call scheduler
    }
}

void lock_release(lock_t * l)
{
    if (SPIN) {
        l->status = UNLOCKED;
    } else {
        // if blocked queue has a process, give it the lock and put it on the ready queue
        // else, owner = -1, status = UNBLOCKED
    }
}
