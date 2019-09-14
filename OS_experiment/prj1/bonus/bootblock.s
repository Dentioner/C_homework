.text
.global main

main:
	# 1) task1 call BIOS print string "It's bootblock!"
	# la	$a0, msg
	# la	$t0, printstr
	# lw	$t1, 0($t0)
	
	la	$a0, msg
	lw	$t1, (printstr)
	jalr	$t1
	nop

	# la	$t0, kernel
	# lw	$a
	# read the size of kernel from the first sector
	lw	$t3, (kernel)
	lb	$t4, -3($t3)
	li	$t5, 0x200
	
	lw	$a0, (kernel)
	li	$a1, 0x200
#	li	$a2, 0x400
	mul	$a2, $t4, $t5
	lw	$t0, (read_sd_card)


	lw	$ra, (kernel_main)
	jr	$t0	
#	lw	$ra, kernel_main
#	j	0x80011000	
	
	# 2) task2 call BIOS read kernel in SD card and jump to kernel start
	
# while(1) --> stop here
stop:
	j stop

.data

msg: .ascii "It's a bootloader...\n"
# info:	.ascii "Hello OS!\n"

# 1. PMON read SD card function address
# read_sd_card();
read_sd_card: .word 0x80011000

# 2. PMON print string function address
# printstr(char *string)
printstr: .word 0x80011100

# 3. PMON print char function address
# printch(char ch)
printch: .word 0x80011140

# 4. kernel address (move kernel to here ~)
# kernel : .word 0xa0800200
kernel : .word 0xa0800000

# 5. kernel main address (jmp here to start kernel main!)
# kernel_main : .word 0xa0800200
kernel_main : .word 0xa0800000
