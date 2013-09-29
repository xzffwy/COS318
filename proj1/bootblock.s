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

	# read drive params
	movb $0x08, %ah
	int $0x13

	# save drive params
	# sectors per track
	movb %cl, %al
	andw $0x1F, %ax
	pushw %ax
	# cylinders per head
	movb %ch, %al
	movb %cl, %ah
	shrb $0x6, %ah
	incw %ax
	pushw %ax
	# number of heads
	movb %dh, %al
	incb %al
	subb %ah, %ah
	pushw %ax

	# save boot drive
	movw %dx, %ax
	pushw %ax


	jmp end

	#load kernel data

	#set extra segment to kernel write destination
	movw $0x100, %ax
	movw %ax, %es

	# read number of sectors provided by createimage and specify for interrupt
	movb os_size, %al
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

end:
	jmp end