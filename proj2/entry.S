# entry.S
##

	.data


	.text
	.code32

	.globl	kernel_entry
kernel_entry:
	popl %eax
	# if eax is 0 do_yield
	# if eax is 1 do_exit
	ret

	.globl	scheduler_entry
scheduler_entry:
	# save pcb shits

	# call scheduler, which will set new current running process

	# push eip of process to run next

	ret # return to new task
