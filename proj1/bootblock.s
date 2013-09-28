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
	#set bootloader stack segment
	movw $0x9000, %ax
	movw %ax, %ss
	#set extra segment to 0x1000
	movw $0x100, %ax
	movw %ax, %es
	#set bootloader base and stack pointer
	movw $0xFFFF, %ax
	movw %ax, %bp
	movw %ax, %sp

	#set data segment
	movw %cs, %ax
	movw %ax, %ds
	#load kernel data
	# read number of sectors provided by createimage and specify for interrupt
	movw $os_size, %si
	lodsw

	movb $DISK_READ, %ah


	#set bios data segment
	movw $0x0, %cx
	movw %cx, %ds

	movw $0x1, %cx #cylinder = 0, sector = 1
	movb $0x00, %dh #head = 0, dl should be as it was set by BIOS

	movw $0x0, %bx #set write destination of kernel


	int $0x13


# setup the kernel stack
setup_stack:	
	#reset stack pointer
	movw %bp, %sp
	#set data segment

	jmp end

# switch control to kernel
switch_to_kernel:
	

	#jump to kernel
	ljmp $0x100, $0x0

end:
