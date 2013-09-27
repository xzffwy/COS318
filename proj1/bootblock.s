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
	movw $0x0000, %ax
	movw %ax, %ss
	#set extra segment to 0
	movw %ax, %es
	#set bootloader base and stack pointer
	mov $0xA0000, %eax
	mov %eax, %ebp
	mov %eax, %esp

	#load kernel data
	movb $0x02, %ah
	movb os_size, %al #sectors to read = 128
	movw $0x0001, %cx #cylinder = 0, sector = 1
	movb $0x00, %dh #head = 0, dl should be as it was set by BIOS

	movw $0x1000, %bx #set write destination of kernel

	enter $0, $0

	int $0x13

	leave


# setup the kernel stack
setup_stack:	
	#set stack segment
	movw $0x0000, %ax
	movw %ax, %ss
	#set base and stack pointer
	movw $0xA000, %ax
	movw %ax, %bp
	movw %ax, %sp

# switch control to kernel
switch_to_kernel:
	#set kernel data segment
	movw $BOOT_SEGMENT, %ax
	movw %ax, %ds

	#jump to kernel
	ljmp 0x1000


# print a character to screen at the position of the cursor. TODO: advance the cursor
print_char:
	push %ebx
	
	mov $0x0e,%ah #specify teletype
	mov $0x00,%bh #page number
	mov $0x02,%bl #color of foreground (white)

	push %edx
	int $0x10 #call interrupt
	pop %edx
		
	pop %ebx
	ret