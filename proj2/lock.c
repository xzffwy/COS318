/* lock.c: mutual exclusion
 * If SPIN is false, lock_acquire() should block the calling process until its request can be satisfied
 * Your solution must satisfy the FIFO fairness property
 *
 *  It is assumed that only one process will attempt to initialize a lock,
 *                that processes will call lock_acquire only on an initialized lock
 *                that processes will call lock_release after lock_acquire (and before exiting)
 *                that processes will release a lock before attempting to reacquire it
 */

#include "common.h"
#include "lock.h"
#include "scheduler.h"

enum {
    SPIN = FALSE,
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
        // case 1: lock is free
        if (l->status == UNLOCKED) {
            l->status = LOCKED;
            return;
        }
        // case 2: lock is owned
        block();
    }
}

void lock_release(lock_t * l)
{
    if (SPIN) {
        l->status = UNLOCKED;
    } else {
        // check for blocked tasks
        if (blocked_tasks())
            unblock(); // unblock next task waiting on lock
        // if no blocked tasks, free lock
        else
            l->status = UNLOCKED;
    }
}
