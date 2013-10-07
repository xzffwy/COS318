/* scheduler.h */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common.h"
#include "kernel.h"

/* Number of times scheduler() was called */
extern int scheduler_count;

/* Save the context and the kernel stack before calling scheduler
 * This function is implemented in entry.S
 */
void scheduler_entry(void);

/* Change current_running to the next task */
void scheduler(void);

/* Schedule another task
 * Call from a kernel thread or kernel_entry_helper()
 */
void do_yield(void);

/* Schedule another task
 * Do not reschedule the current one
 * Call from a kernel thread or kernel_entry_helper()
 */
void do_exit(void);

/* Put the currently running task on the blocked queue */
void block(void);

/* If there is a task on the blocked queue, move it to the ready queue */
void unblock(void);

/* Returns TRUE if there are blocked tasks, FALSE otherwise */
bool_t blocked_tasks(void);

#endif                          /* SCHEDULER_H */
