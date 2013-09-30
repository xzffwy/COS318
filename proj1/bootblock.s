# Author(s): <Your name here>
# COS 318, Fall 2013: Project 1 Bootloader
# bootloader
# Largely unimplemented

# .equ symbol, expression
# These directive set the value of the symbol to the expression
# memory constants
.equ  BOOT_SEGMENT,0x7c0
# more memory constants...

# utility constants
.equ  DISK_READ,0x02
# more utility constants...

.text               #Code segment
.globl    _start    #The entry point must be global
.code16             #Real mode

#
# The first instruction to execute in a program is called the entry
# point. The linker expects to find the entry point in the "symbol" _start
# (with underscore).
#

_start:
  jmp load_kernel

# Area reserved for createimage to write the OS size
os_size:
	.word   0
	.word   0

# setup registers for kernel data and disk read	
load_kernel:  	
	#set data segment
	movw $BOOT_SEGMENT, %ax
	movw %ax, %ds

	#set bootloader stack segment
	movw $0x9000, %ax
	movw %ax, %ss

	#set bootloader base and stack pointer
	movw $0xFFFF, %ax
	movw %ax, %bp
	movw %ax, %sp

	# save boot drive number
	pushw %dx

	# move kernel
	# set extra segment to new bootblock destination
	movw $0x9000, %ax
	movw %ax, %es

	# read 1 sector
	movb $0x1, %al
	# specify interrupt function
	movb $DISK_READ, %ah

	# get the driver saved from earlier
	popw %dx
	pushw %dx
	movb $0x00, %dh #head = 0

	# set data segment for bios
	movw $0x0, %cx
	movw %cx, %ds

	movw $0x1, %cx #cylinder = 0, sector = 1

	movw $0xFD00, %bx #set write destination of new bootblock

	int $0x13

	# jump to write_kernel in new kernel
	ljmp $0x9FD0, $write_kernel

write_kernel:

	#load kernel data

	# set data segment for new kernel
	movw $0x9FD0, %cx
	movw %cx, %ds	

	#set extra segment to kernel write destination
	movw $0x100, %ax
	movw %ax, %es

	# read number of sectors provided by createimage and specify for interrupt
	movb os_size, %al
	# specify interrupt function
	movb $DISK_READ, %ah

	# get the driver saved from earlier
	popw %cx
	movw %cx, %dx
	movb $0x00, %dh #head = 0

	# set data segment for bios
	movw $0x0, %cx
	movw %cx, %ds

	movw $0x2, %cx #cylinder = 0, sector = 2

	movw $0x0, %bx #set write destination of kernel

	int $0x13

# setup the kernel stack
setup_stack:	
	# reset stack pointer
	movw %bp, %sp

# switch control to kernel
switch_to_kernel:
	# set data segment
	movw $0x0, %ax
	movw %ax, %ds

	#jump to kernel
	ljmp $0x100, $0x0
