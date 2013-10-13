#include "common.h"
#include "scheduler.h"
#include "util.h"

#define MAXROW 25

uint64_t recorded_time;
char * display_str = "Cycles required per do_yield(): ";


// Thread 4 active
//     record a time; yield immediately
// Thread 5 is next on queue (queue initialized round-robin), becomes active
//     record a time; display difference in times

void thread4(void)
{
	recorded_time = get_timer();
	do_yield();
	do_exit();
}

void thread5(void)
{
	recorded_time = get_timer() - recorded_time;

	print_str(MAXROW-2, 0, display_str);
	print_int(MAXROW-2, strlen(display_str), (int) recorded_time);

	do_exit();
}
