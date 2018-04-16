# File:		build.asm
# Author:	K. Reek
# Contributors:	P. White,
#		W. Carithers,
#		<<<Erikson Sodergren>>>
#
# Description:	Binary tree building functions.
#
# Revisions:	$Log$


	.text			# this is program code
	.align 2		# instructions must be on word boundaries

# 
# Name:		add_elements
#
# Description:	loops through array of numbers, adding each (in order)
#		to the tree
#
# Arguments:	a0 the address of the array
#   		a1 the number of elements in the array
#		a2 the address of the root pointer
# Returns:	none
#

	.globl	add_elements
	
add_elements:
	addi 	$sp, $sp, -16
	sw 	$ra, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

#***** BEGIN STUDENT CODE BLOCK 1 ***************************
#
# Insert your code to iterate through the array, calling build_tree
# for each value in the array.  Remember that build_tree requires
# two parameters:  the address of the variable which contains the
# root pointer for the tree, and the number to be inserted.
#
# Feel free to save extra "S" registers onto the stack if you need
# more for your function.
#
	move	$s0, $a0
	move	$s1, $a1	#count
	move	$s2, $a2
loop_add:
	beq	$zero, $s1, add_done
	move	$a0, $s2	#root for buildtree
	lw	$a1, 0($s0)	#retrieve value
	addi	$s1, $s1, -1
	addi	$s0, $s0, 4
	jal	build_tree
	j	loop_add
#
# If you saved extra "S" reg to stack, make sure you restore them
#

#***** END STUDENT CODE BLOCK 1 *****************************

add_done:

	lw 	$ra, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 16
	jr 	$ra

#***** BEGIN STUDENT CODE BLOCK 2 ***************************
#
# Put your build_tree subroutine here.
#

	.globl	build_tree
build_tree:
	.globl	allocate_mem
	addi 	$sp, $sp, -20
	sw 	$ra, 16($sp)
	sw	$s3, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

	move	$s0, $a0
	move	$s1, $a1
	move	$t2, $s2
	lw	$s2, 0($s0)
build_loop:
	beq	$s2, $zero, insert	#if empty, just insert
	lw	$t0, 0($s2)
	beq	$t0, $s1, build_done	#dont if value already in
	slt	$t1, $s1, $t0
	beq	$t1, $zero, move_right
move_left:
	addi	$t2, $s2, 4	
	lw	$s2, 4($s2)
	j	build_loop
move_right:
	addi	$t2, $s2, 8
	lw	$s2, 8($s2)
	j	build_loop
insert:
	move	$s2, $t2
	li	$a0, 3		#allocate mem and udate
	jal	allocate_mem
	move	$s3, $v0
	sw	$s1, 0($s3)
	sw	$s3, 0($s2)	#be sure to set s2 to the pointer to update

build_done:
	lw 	$ra, 16($sp)
	lw	$s3, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 20
	jr 	$ra
#***** END STUDENT CODE BLOCK 2 *****************************
