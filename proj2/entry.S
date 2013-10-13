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
	# set stack to current_running at where eflags should be
	movl current_running, %ecx
	leal 4(%ecx), %esp
	# pop eflags off stack
	popfl
	# pop all the other registers (omitting esp) off stack
	popal

	# get esp!
	leal 20(%esp), %esp
	popl %esp

	# push eip of process to run next
	pushl (current_running)

	# return to new task
	ret

# Saves the pcb of the currently running process. Use EIP from two stack calls up (should be esp + 8)
	.globl save_pcb
save_pcb:

	# push all the registers on the stack
	pushal

	# save eip
	movl 40(%esp), %ecx
	movl %ecx, current_running
	# save correct esp from where do_yield was called where eflags should be
	leal 40(%esp), %eax
	movl current_running, %ecx
	movl %eax, 4(%ecx)

	# restore registers
	popal

	# set stack to current_address at where eax should be
	movl current_running, %esp
	leal 40(%esp), %esp
	# save registers
	pushal
	# move esp stored in eflags spot to esp spot
	movl current_running, %esp
	movl 4(%esp), %eax
	movl %eax, 20(%esp)
	# save eflags
	addl $0x8, %esp
	pushfl
	# set stack back
	movl current_running, %eax
	movl 20(%eax), %esp
	subl $0x8, %esp

	ret
