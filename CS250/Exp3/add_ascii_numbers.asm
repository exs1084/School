# File:		add_ascii_numbers.asm
# Author:	K. Reek
# Contributors:	P. White, W. Carithers
#		<<Erikson Sodergren>>
#
# Updates:
#		3/2004	M. Reek, named constants
#		10/2007 W. Carithers, alignment
#		09/2009 W. Carithers, separate assembly
#
# Description:	Add two ASCII numbers and store the result in ASCII.
#
# Arguments:	a0: address of parameter block.  The block consists of
#		four words that contain (in this order):
#
#			address of first input string
#			address of second input string
#			address where result should be stored
#			length of the strings and result buffer
#
#		(There is actually other data after this in the
#		parameter block, but it is not relevant to this routine.)
#
# Returns:	The result of the addition, in the buffer specified by
#		the parameter block.
#

	.globl	add_ascii_numbers

add_ascii_numbers:
A_FRAMESIZE = 40

#
# Save registers ra and s0 - s7 on the stack.
#
	addi 	$sp, $sp, -A_FRAMESIZE
	sw 	$ra, -4+A_FRAMESIZE($sp)
	sw 	$s7, 28($sp)
	sw 	$s6, 24($sp)
	sw 	$s5, 20($sp)
	sw 	$s4, 16($sp)
	sw 	$s3, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)
	
# ##### BEGIN STUDENT CODE BLOCK 1 #####
	lw	$s1, 0($a0)	#add1 in s1
	lw	$s2, 4($a0)	#add2 in s2
	lw	$s0, 8($a0)	#ret string add in s0
	lw	$s3, 12($a0)	#length in s3
	add	$t0, $zero, $s3	#set countdown
	addi	$t1, $t0, -1
	add	$s0, $t1
	add	$s1, $t1
	add	$s2, $t1	#set addr to last chars
	move	$t4, $zero	#init carry
	addi	$t5, $t5, 1	#change carry indicator
loop:
	beq	$t0, $zero, done#done if countdown=0
	lb	$t1, 0($s1)
	lb	$t2, 0($s2)	#load digits to add
	add	$t3, $t1, $t2	#add
	addi	$t3, $t3, -48	#account for ascii math
	bne	$t5, $zero, no_prev_car#t5=0 means carried
	addi	$t3, $t3, 1	#add previous carry
	addi	$t5, $t5, 1	#change carry indicator
no_prev_car:
	addi	$t4, $t3, -57	#find if carry
	slti	$t5, $t4, 1	#if not, t5=1
	bne	$t5, $zero, no_curr_car
	addi	$t3, $t3, -10	#sub 10 cuz carry
no_curr_car:
	sb	$t3, 0($s0)	#store sum
	addi	$t0, $t0, -1
	addi	$s0, $s0, -1
	addi	$s1, $s1, -1
	addi	$s2, $s2, -1
	j	loop
done:
# ###### END STUDENT CODE BLOCK 1 ######

#
# Restore registers ra and s0 - s7 from the stack.
#
	lw 	$ra, -4+A_FRAMESIZE($sp)
	lw 	$s7, 28($sp)
	lw 	$s6, 24($sp)
	lw 	$s5, 20($sp)
	lw 	$s4, 16($sp)
	lw 	$s3, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, A_FRAMESIZE

	jr	$ra			# Return to the caller.
