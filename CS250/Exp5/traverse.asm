# File:		traverse_tree.asm
# Author:	K. Reek
# Contributors:	P. White,
#		W. Carithers,
#		<<<Erikson Sodergren>>>
#
# Description:	Binary tree traversal functions.
#
# Revisions:	$Log$


# CONSTANTS
#

# traversal codes
PRE_ORDER  = 0
IN_ORDER   = 1
POST_ORDER = 2

	.text			# this is program code
	.align 2		# instructions must be on word boundaries

#***** BEGIN STUDENT CODE BLOCK 3 *****************************
#
# Put your traverse_tree subroutine here.
# 
	.globl	traverse_tree
traverse_tree:
	addi 	$sp, $sp, -16
	sw 	$ra, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

	move	$s0, $a0
	move	$s1, $a1
	move	$s2, $a2
	beq	$s0, $zero, traverse_done
	li	$t0, PRE_ORDER
	beq	$t0, $s2, trav_pre
	li	$t0, IN_ORDER
	beq	$t0, $s2, trav_in
	li	$t0, POST_ORDER
	beq	$t0, $s2, trav_post
trav_pre:	#print, left, right
	#move	$a0, $s0
	jalr	$s1
	lw	$a0, 4($s0)
	jal	traverse_tree
	lw	$a0, 8($s0)
	jal	traverse_tree
	j	traverse_done
trav_in:	#left, print, right
	lw	$a0, 4($s0)
	jal	traverse_tree
	move	$a0, $s0
	jalr	$s1
	lw	$a0, 8($s0)
	jal	traverse_tree
	j	traverse_done
trav_post:	#left, right, print
	lw	$a0, 4($s0)
	jal	traverse_tree
	lw	$a0, 8($s0)
	jal	traverse_tree
	move	$a0, $s0
	jalr	$s1
traverse_done:
	lw 	$ra, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 16

	jr	$ra

#***** END STUDENT CODE BLOCK 3 *****************************
