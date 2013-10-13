#include "common.h"
#include "syslib.h"
#include "util.h"

#define MAXROW 25

char * display_str = "Cycles required per yield(): ";

void _start(void)
{
	uint64_t recorded_time;
	recorded_time = get_timer();
	yield();
	recorded_time = get_timer() - recorded_time;
	print_str(MAXROW-1, 0, display_str);
	print_int(MAXROW-1, strlen(display_str), (int) recorded_time);
    exit();
}
