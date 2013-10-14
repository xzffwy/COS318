# entry.S
##

	.data


	.text
	.code32

	.globl	kernel_entry
kernel_entry:
	movl %ecx, %eax
	movl 4(%esp), %ecx
	# do_yield if ecx is 0
	jecxz no_exit

exit_process:
	movl %eax, %ecx
	# we're assuming that if %eax is not 0, then it is 1
	call do_exit
	ret

no_exit:
	movl %eax, %ecx
	pushl %ebp
	movl %esp, %ebp
	call do_yield
	popl %ebp
	ret


	.globl	scheduler_entry
scheduler_entry:
	# call scheduler, which will set new current running process
	call scheduler

	# push next eip onto stack of next process
	movl current_running, %ecx
	movl %esp, %eax # stash esp on eax
	movl 20(%ecx), %esp
	pushl (%ecx) # push the value of eip for return to stack pointer of next process which we will switch to down there
	pushl 32(%ecx) # push what the value of ecx should be
	movl %esp, 32(%ecx) # store esp where we would store ecx otherwise
	movl %eax, %esp # restore esp from eax

	# set stack to current_running at where eflags should be
	movl current_running, %ecx #J: relevant eip in ecx
	leal 4(%ecx), %esp #J: esp has address of eflags

	# pop eflags off stack
	popfl #loads flags from relevant address, sets stack pointer to above register values
	
	# pop all the other registers (omitting esp) off stack
	popal # loads registers

	# get esp!
	movl %ecx, %esp # ecx that was popped off actually stored value of esp
	popl %ecx # pop value of ecx off stack that was stored before

	# set esp to be below eip of next thread
	# note we pushed eip above and set esp to point to below that point, so we are good

	# return to new task
	ret

# Saves the pcb of the currently running process. Use EIP from above stack base pointer
	.globl save_pcb
save_pcb:
	# push some registers to clear temp var space
	pushl %ecx
	pushl %eax

	# save eip
	movl 4(%ebp), %ecx
	movl current_running, %eax
	movl %ecx, (%eax)

	# save correct esp from where do_yield. use the eflags entry as temp storage
	leal 8(%ebp), %ecx
	movl %ecx, 4(%eax)

	# restore registers
	popl %eax
	popl %ecx

	# save old stack pointer
	pushl %esp
	# save old base pointer
	pushl %ebp
	# set base pointer to esp
	movl %esp, %ebp

	# set stack to current_running offset to where eax should be
	movl current_running, %esp
	leal 40(%esp), %esp

	# save registers
	pushal

	# move esp value stored in eflags spot to correct esp spot
	movl current_running, %esp
	movl 4(%esp), %eax
	movl %eax, 20(%esp)

	# store correct ebp value
	movl (%ebp), %eax
	movl (%eax), %eax
	movl %eax, 16(%esp)

	# save eflags
	leal 8(%esp), %esp
	pushfl

	# set stack back
	movl %ebp, %esp
	movl (%ebp), %ebp
	popl %eax
	popl %esp

	ret
