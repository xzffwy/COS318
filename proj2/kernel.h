/* kernel.h: definitions used by kernel code */

#ifndef KERNEL_H
#define KERNEL_H

#define NUM_REGISTERS 8

#include "common.h"

/* ENTRY_POINT points to a location that holds a pointer to kernel_entry */
#define ENTRY_POINT ((void (**)(int)) 0x0f00)

/* System call numbers */
enum {
    SYSCALL_YIELD,
    SYSCALL_EXIT,
};

/* All stacks should be STACK_SIZE bytes large
 * The first stack should be placed at location STACK_MIN
 * Only memory below STACK_MAX should be used for stacks
 */
enum {
    STACK_MIN = 0x10000,
    STACK_SIZE = 0x1000,
    STACK_MAX = 0x20000,
};

typedef enum {
	PROCESS_BLOCKED,
	PROCESS_READY,
	PROCESS_RUNNING,
	PROCESS_EXITED,
} process_state;

typedef struct pcb {
	uint32_t eip;
	uint32_t eflags;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	bool_t isKernel;
	process_state state;
} pcb_t;

/* The task currently running */
extern pcb_t *current_running;

void kernel_entry(int fn);

#endif                          /* KERNEL_H */
