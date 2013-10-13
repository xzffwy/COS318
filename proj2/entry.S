# entry.S
##

	.data


	.text
	.code32

	.globl	kernel_entry
kernel_entry:
	movl 8(%esp), %eax
	// if eax is 0 do_yield
	cmpl $0x0, %eax
	jne exit_process
	call do_yield
	ret
exit_process: // we're assuming that if %eax is not 0, then it is 1
	call do_exit
	ret

	.globl	scheduler_entry
scheduler_entry:
	# call scheduler, which will set new current running process
	call scheduler

	# set stack to current_running at where eflags should be
	leal 4(current_running), %esp
	# pop eflags off stack
	popfl
	# pop all the other registers (omitting esp) off stack
	popal
	movl 20(current_running), %esp

	# push eip of process to run next
	pushl (current_running)

	# return to new task
	ret

/* Saves the pcb of the currently running process. Use EIP from two stack calls up (should be esp + 8) */
	.globl save_pcb
save_pcb:
	# save EIP from where do_yield was called
	movl 8(%esp), current_address
	# save correct esp from where do_yield was called
	leal 8(%esp), %esp
	movl %esp, 4(current_address)
	# set stack to current_running at where eax should be
	leal 40(current_running), %esp
	# save registers
	pushal
	# move esp stored in eflags spot to esp spot
	movl 4(current_address), 20(current_address)
	# save eflags
	pushfl
	# set stack back
	movl 20(current_address), %esp
	decl $0x8, %esp

	ret
