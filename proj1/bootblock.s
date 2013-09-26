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
	movw $0x07c0, %ax
	movw %ax, %ss
	#set bootloader base and stack pointer
	movw $0x10, %ax
	movw %ax, %bp
	movw %ax, %sp

	#load kernel data
	movb $0x02, %ah
	movb $0x01, %al #sectors to read







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
	movw $0x07c0, %ax
	movw %ax, %ds

	

# print a character to screen at the position of the cursor. TODO: advance the cursor
print_char:
	enter $0,$0

	mov $0x0e,%ah #specify teletype
	mov $0x00,%bh #page number
	mov $0x02,%bl #color of foreground (white)

	int $0x10 #call interrupt
		
	leave
	ret
