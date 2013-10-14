#include "common.h"
#include "scheduler.h"
#include "util.h"

uint64_t rec_time;
char * display_str = "Cycles required per do_yield(): ";


// Thread 4 active
//     record a time; yield immediately
// Thread 5 is next on queue (queue initialized round-robin), becomes active
//     record a time; display difference in times

void thread4(void)
{
	rec_time = get_timer();
	do_yield();
	do_exit();
}

void thread5(void)
{
	rec_time = get_timer() - rec_time;

	print_str(0, 0, display_str);
	print_int(0, strlen(display_str), (int) rec_time);

	do_exit();
}
