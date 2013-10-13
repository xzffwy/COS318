# entry.S
##

	.data


	.text
	.code32

	.globl	kernel_entry
kernel_entry:
	movl 8(%esp), %eax
	# do_yield if eax is 0
	cmpl $0x0, %eax
	jne exit_process
	call do_yield
	ret
exit_process:
	# we're assuming that if %eax is not 0, then it is 1
	call do_exit
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

# Saves the pcb of the currently running process. Use EIP from two stack calls up (should be esp + 8)
	.globl save_pcb
save_pcb:
	xchg %bx,%bx
	# push all the registers on the stack
	pushal
save_pcb1:
	xchg %bx,%bx
	# save eip
	movl 4(%ebp), %ecx
	movl current_running, %eax
	movl %ecx, (%eax)
save_pcb2:
	# save correct esp from where do_yield. use the eflags entry as temp storage
	leal 8(%ebp), %ecx
	movl %ecx, 4(%eax)
save_pcb3:
	# restore registers
	popal
	# save old stack pointer
	pushl %esp
	# save old base pointer
	pushl %ebp
	# set base pointer to esp
	movl %esp, %ebp
save_pcb4:
	# set stack to current_address at where eax should be
	movl current_running, %esp
	leal 40(%esp), %esp
save_pcb5:
	# save registers
	pushal
save_pcb6:
	# move esp value stored in eflags spot to correct esp spot
	movl current_running, %esp
	movl 4(%esp), %eax
	movl %eax, 20(%esp)
save_pcb7:
	# store correct ebp value
	movl (%ebp), %eax
	movl (%eax), %eax
	movl %eax, 16(%esp)
save_pcb8:
	# save eflags
	leal 8(%esp), %esp  # J: subtract off 4?
	pushfl
save_pcb9:
	# set stack back
	movl %ebp, %esp
	movl (%ebp), %ebp
	popl %eax
	popl %esp
save_pcb10:

	ret
