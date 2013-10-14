#include "common.h"
#include "syslib.h"
#include "util.h"

#define MAXROW 25

#define INT_MAX_VAL 18446744073709551615U

char * display_str = "Cycles required per yield(): ";
static uint64_t recorded_time = INT_MAX_VAL; //maxval of 64-bit int

void _start(void)
{
	uint64_t time;
	/* grab time immediately so other calculations don't come into play */
	time = get_timer();
	
	/* if this is the first process to come about, record time, yield, and exit */
	if (time < recorded_time) {
		recorded_time = get_timer();
		yield();
		exit();
	}
	/* second process calculates difference in time then exits */
	else {
		print_str(0, strlen(display_str) + 10, display_str);
		print_int(0, 2 * strlen(display_str) + 10, (int) (time - recorded_time));
    	exit();
	}
	
}
