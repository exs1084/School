# File:		$Id$
# Author:	Erikson Sodergren

# CONSTANTS
#
# syscall codes
PRINT_INT = 	1
PRINT_STRING = 	4
PRINT_CHAR = 	11
READ_INT = 	5
EXIT = 		10

	.data
	.align 2
row_A:
	.asciiz "......................................................................"

row_B:
	.asciiz "......................................................................"

rule:
	.word 30

gens:
	.word 0
curr_gen:
	.word 0
size:
	.word 7

new_line:
	.asciiz	"\n"

space:
	.asciiz " "
o_paren:
	.asciiz "("
c_paren:
	.asciiz ")"

rule_error:
	.asciiz "Invalid rule number, cell-auto-sim terminating\n"

gen_error:
	.asciiz "Invalid generation number, cell-auto-sim terminating\n"

size_error:
	.asciiz "Invalid row size, cell-auto-sim terminating\n"

input_error:
	.asciiz "Illegal input value, cell-auto-sim terminating\n"

banner:
	.ascii "*****************************************\n"
	.ascii "**     Cellular Automata Simulator     **\n"
	.asciiz "*****************************************\n\n"

rule_out:
	.asciiz "Rule "
scale:
	.asciiz "   ----+----0----+----0----+----0----+----0----+----0----+----0----+----0"   
	
	.text

# 
# Name:		print_int
#
# Description:	just prints an integer
#
# Arguments:	a0 the address of the number to print
# Returns:	none
#
print_int:
	li 	$v0, PRINT_INT
	lw	$a0, 0($a0)
	syscall
	jr	$ra

# 
# Name:		print_gen
#
# Description:	just prints the generation
#
# Arguments:	none
# Returns:	none
#
print_gen:
	addi 	$sp, $sp, -4  	# allocate space for the return address
	sw 	$ra, 0($sp)	# store the ra on the stack

	la	$t0, curr_gen
	lw	$t1, 0($t0)
	slti	$t2, $t1, 10
	beq	$t2, $zero, no_space
single_space:
	la	$a0, space
	jal	print_str
no_space:
	move	$a0, $t0
	jal	print_int
	la	$a0, space
	jal	print_str

	lw 	$ra, 0($sp)
	addi 	$sp, $sp, 4 
	jr	$ra


# 
# Name:		print_char
#
# Description:	just prints a char
#
# Arguments:	a0 the address of the char to print
# Returns:	none
#
print_char:
	li 	$v0, PRINT_CHAR
	#lw	$a0, 0($a0)
	syscall
	jr	$ra

# 
# Name:		print_str
#
# Description:	just prints a string
#
# Arguments:	a0 the address of the string to print
# Returns:	none
#
print_str:
	li 	$v0, PRINT_STRING
	syscall		
	jr	$ra
# 
# Name:		print_bin
#
# Description:	just prints an 8-bit rule in binary
#
# Arguments:	a0 the address of the int to print
# Returns:	none
#
print_bin:
	addi 	$sp, $sp, -16  	# allocate space for the return address
	sw 	$ra, 12($sp)	# store the ra on the stack
	sw	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

	lw	$s0, 0($a0)
	addi	$s1, $zero, 7
bin_loop:
	srlv	$s2, $s0, $s1
	andi	$a0, $s2, 1
	li 	$v0, PRINT_INT
	syscall
	beq	$s1, $zero, end_bin
	addi	$s1, -1
	j	bin_loop
end_bin:
	lw 	$ra, 12($sp)
	lw	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 16   	# deallocate space for the return address
	jr	$ra
	
	
print_start:
	addi 	$sp, $sp, -4  	# allocate space for the return address
	sw 	$ra, 0($sp)	# store the ra on the stack

	la	$a0, banner
	jal	print_str
	la	$a0, rule_out
	jal	print_str
	la	$a0, rule
	jal	print_int
	la	$a0, space
	jal	print_str
	la	$a0, o_paren
	jal	print_str
	la	$a0, rule
	jal	print_bin
	la	$a0, c_paren
	jal	print_str
	la	$a0, new_line
	jal	print_str
	li	$a0, 2
	jal	print_row

	lw 	$ra, 0($sp)
	addi 	$sp, $sp, 4   	# deallocate space for the return address
	jr 	$ra		# return from main and exit
# 
# Name:		print_row
#
# Description:	just prints an row or scale
#
# Arguments:	a0     0=rowB, 1=rowA, 2=scale
# Returns:	none
#
print_row:
	addi 	$sp, $sp, -16  	# allocate space for the return address
	sw 	$ra, 12($sp)	# store the ra on the stack
	sw	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

	move	$s0, $a0
	beq	$s0, $zero, print_B
	addi	$s0, $s0, -2
	beq	$s0, $zero, print_scale
print_A:
	jal	print_gen
	la	$a0, row_A
	j	prow_cont
print_scale:
	la	$a0, scale
	j	prow_cont
print_B:
	jal	print_gen
	la	$a0, row_B
prow_cont:
	jal	print_str
prow_done:
	la	$a0, new_line
	jal	print_str

	lw 	$ra, 12($sp)
	lw	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 16   	# deallocate space for the return address
	jr	$ra

# 
# Name:		rule_#(0-7)
#
# Description:	applies the given rule bit
#
# Arguments:	a0     left cell, a1	current cell, a2	right cell
# Returns:	v0	updated current cell
#
rule_0:
	la	$t0, rule
	lw	$t0, 0($t0)
	j	live_or_not
rule_1:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 1
	j	live_or_not
rule_2:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 2
	j	live_or_not
rule_3:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 3
	j	live_or_not
rule_4:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 4
	j	live_or_not
rule_5:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 5
	j	live_or_not
rule_6:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 6
	j	live_or_not
rule_7:
	la	$t0, rule
	lw	$t0, 0($t0)
	srl	$t0, $t0, 7
live_or_not:
	andi	$t0, $t0, 1
	beq	$t0, $zero, dead	#should it live or die?
live:
	addi	$t1, $a1, -46
	beq	$t1, $zero, birth
	addi	$v0, $a1, 1
	j	done_rule
birth:
	li	$v0, 65
	j	done_rule
dead:
	li	$v0, 46
done_rule:
	jr	$ra


# 
# Name:		apply_rules
#
# Description:	loops over a row and applies the rule
#
# Arguments:	a0     which row to update		0=B, 1=A
# Returns:	none
#
apply_rules:
	addi 	$sp, $sp, -28  	# allocate space for the return address
	sw 	$ra, 24($sp)	# store the ra on the stack
	sw 	$s5, 20($sp)
	sw 	$s4, 16($sp)
	sw 	$s3, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

	bne	$a0, $zero, change_A
change_B:
	la	$s0, row_B
	la	$s1, row_A
	la	$s5, row_A
	j	change_set
change_A:
	la	$s0, row_A
	la	$s1, row_B
	la	$s5, row_B
change_set:	#s1 will remain, s0 will be updated
	la	$s2, size
	lw	$s2, 0($s2)
	move	$s3, $zero	#counter
apply_loop:
	beq	$s3, $s2, apply_last
	beq	$s3, $zero, apply_first
	lb	$a0, -1($s1)	#left
	lb	$a1, 0($s1)	#current
	lb	$a2, 1($s1)	#right
	j	pickrule
apply_first:
	add	$t7, $s1, $s2
	addi	$t7, $t7, -1
	addi	$s3, $s3, 1	#incremement as a bugfix?
				#ok, it was looping too far(to the null)
	lb	$a0, 0($t7)	#left
	lb	$a1, 0($s1)	#current
	lb	$a2, 1($s1)	#right
	j	pickrule
apply_last:
	lb	$a0, -1($s1)	#left
	lb	$a1, 0($s1)	#current
	lb	$a2, 0($s5)	#right
	j	pickrule

pickrule:
	li	$s4, 46		#period ascii
	beq	$a0, $s4, r0xx
r1xx:
	beq	$a1, $s4, r10x
r11x:
	beq	$a2, $s4, r110
	jal	rule_7
	j	apply_increment
r110:
	jal	rule_6
	j	apply_increment
r10x:
	beq	$a2, $s4, r100
	jal	rule_5
	j	apply_increment
r100:
	jal	rule_4
	j	apply_increment
r0xx:
	beq	$a1, $s4, r00x
r01x:
	beq	$a2, $s4, r010
	jal	rule_3
	j	apply_increment
r010:	
	jal	rule_2
	j	apply_increment
r00x:
	beq	$a2, $s4, r000
	jal	rule_1
	j	apply_increment
r000:
	jal	rule_0
apply_increment:
	sb	$v0, 0($s0)
	beq	$s3, $s2, apply_done
	addi	 $s3, $s3, 1
	addi	$s0, $s0, 1
	addi	$s1, $s1, 1
	j	apply_loop
apply_done:
	lw 	$ra, 24($sp)
	lw 	$s5, 20($sp)
	lw 	$s4, 16($sp)
	lw 	$s3, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 28   	# deallocate space for the return address
	jr 	$ra		# return from main and exit
#
# Name:		MAIN PROGRAM
#
# Description:	Main logic for the program.
#
#	This program 
#
	.globl	main
main:
	addi 	$sp, $sp, -12  	# allocate space for the return address
	sw 	$ra, 8($sp)	# store the ra on the stack
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)
main_read:
	li	$v0, READ_INT	#rule number
	syscall
	move	$s0, $v0
	slt	$t0, $v0, $zero
	bne	$zero, $t0, error1
	slti	$t0, $v0, 256
	beq	$zero, $t0, error1
	la	$t0, rule
	sw	$s0, 0($t0)

	li	$v0, READ_INT	#generations number
	syscall
	move	$s0, $v0
	slt	$t0, $v0, $zero
	bne	$zero, $t0, error2
	slti	$t0, $v0, 26
	beq	$zero, $t0, error2
	la	$t0, gens
	sw	$s0, 0($t0)

	li	$v0, READ_INT	#size
	syscall
	move	$s0, $v0
	move	$s2, $v0
	slti	$t0, $v0, 1
	bne	$zero, $t0, error3
	slti	$t0, $v0, 71
	beq	$zero, $t0, error3
	la	$t1, size
	sw	$s0, 0($t1)
	la	$t1, row_A
	add	$t1, $t1, $s0
	sb	$zero, 0($t1)
	la	$t1, row_B
	add	$t1, $t1, $s0
	sb	$zero, 0($t1)
	addi	$s0, $s0, 3
	la	$t1, scale
	add	$t1, $t1, $s0
	sb	$zero, 0($t1)

			#read actual values(s2 has size)
	move	$s0, $zero	#counter
	la	$s1, row_A
input_loop:
	li	$v0, READ_INT
	syscall
	slti	$t0, $v0, 0
	bne	$zero, $t0, error4
	slti	$t0, $v0, 2
	beq	$zero, $t0, error4
	beq	$v0, $zero, init_dead
init_life:
	li	$t0, 65	#A
	sb	$t0, 0($s1)
	j	in_done
init_dead:
	li	$t0, 46	#period
	sb	$t0, 0($s1)
in_done:
	addi	$s0, $s0, 1
	addi	$s1, $s1, 1
	bne	$s0, $s2, input_loop
begin_main:
	jal	print_start
	li	$a0, 1
	jal	print_row
	li	$s0, 1	#row A
loop_gens:
	beq	$s0, $zero, now_A
	li	$s0, 0
	j	update_gen
now_A:
	li	$s0, 1
update_gen:
	la	$t0, curr_gen
	lw	$t1, 0($t0)
	la	$t2, gens
	lw	$t3, 0($t2)
	beq	$t1, $t3, last_scale
	addi	$t1, $t1, 1
	sw	$t1, 0($t0)
	move	$a0, $s0
	jal	apply_rules
	move	$a0, $s0
	jal	print_row
	j	loop_gens
last_scale:
	li	$a0, 2
	jal	print_row
main_done:
	lw 	$ra, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 12   	# deallocate space for the return address
	jr 	$ra		# return from main and exit
error1:
	la	$a0, rule_error
	jal	print_str
	j	main_done
error2:
	la	$a0, gen_error
	jal	print_str
	j	main_done
error3:
	la	$a0, size_error
	jal	print_str
	j	main_done
error4:
	la	$a0, input_error
	jal	print_str
	j	main_done
