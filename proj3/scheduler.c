/* Author(s): <Your name here>
 * COS 318, Fall 2013: Project 3 Pre-emptive Scheduler
 * Implementation of the process scheduler for the kernel.
*/

#include "common.h"
#include "interrupt.h"
#include "queue.h"
#include "printf.h"
#include "scheduler.h"
#include "util.h"
#include "syslib.h"

pcb_t *current_running;
node_t ready_queue;
node_t sleep_wait_queue;
// more variables...
volatile uint64_t time_elapsed;

/* TODO: Round-robin scheduling: Save current_running before preempting */
void put_current_running(){
  // TO-DO: Do we want to wake up sleeping processes here as well?
  enqueue(&ready_queue, (node_t *)current_running);
}

/* Change current_running to the next task */
void scheduler(){
  ASSERT(disable_count);
  check_sleeping(); // wake up sleeping processes
  while (is_empty(&ready_queue)){     
    leave_critical();
    enter_critical();
  }
  current_running = (pcb_t *) dequeue(&ready_queue);
  ASSERT(NULL != current_running);
  ++current_running->entry_count;
}

int lte_deadline(node_t *a, node_t *b) {
  pcb_t *x = (pcb_t *)a;
  pcb_t *y = (pcb_t *)b;

  if (x->deadline <= y->deadline) {
    return 1;
  } else {
    return 0;
  }
}

/* TODO: Blocking sleep. Caution: this function currently cannot be pre-empted! */
void do_sleep(int milliseconds){
  ASSERT( !disable_count );
  
  enter_critical();
  uint64_t deadline;
  
  deadline = time_elapsed + milliseconds;
  
  current_running->deadline = deadline;

  enqueue_sort(&sleep_wait_queue, (node_t *)current_running, (node_lte)&lte_deadline);

  scheduler_entry();
  leave_critical(); 
}

/* TODO: Check if we can wake up sleeping processes */
void check_sleeping(){
  pcb_t *sleeping;

  while (!is_empty(&sleep_wait_queue)) {
    sleeping = (pcb_t *)peek(&sleep_wait_queue);
    if (sleeping->deadline >= time_elapsed) {
      enqueue(&ready_queue, dequeue(&sleep_wait_queue));
    } else {
      return;
    }
  }

}

/* Do not modify any of the following functions */

void do_yield(){
  enter_critical();
  put_current_running();
  scheduler_entry();
  leave_critical();
}

void do_exit(){
  enter_critical();
  current_running->status = EXITED;
  scheduler_entry();
  /* No need for leave_critical() since scheduler_entry() never returns */
}

void block(node_t * wait_queue){
  ASSERT(disable_count);
  current_running->status = BLOCKED;
  enqueue(wait_queue, (node_t *) current_running);
  scheduler_entry();
}

void unblock(pcb_t * task){
  ASSERT(disable_count);
  task->status = READY;
  enqueue(&ready_queue, (node_t *) task);
}

pid_t do_getpid(){
  pid_t pid;  
  enter_critical();
  pid = current_running->pid;
  leave_critical();
  return pid;
}

uint64_t do_gettimeofday(void){
  return time_elapsed;
}

/* Extra credit */
priority_t do_getpriority(){
  priority_t priority;  
  enter_critical();
  priority = current_running->priority;
  leave_critical();
  return priority;
}

/* Extra credit */
void do_setpriority(priority_t priority){
  if( priority >= 1 ){
    enter_critical();
    current_running->priority = priority;
    leave_critical();
  }
}

