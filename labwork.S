  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

	# THIS FILE WAS MODIFIED BY ERIK LOSIN & ISAK AHLBERG

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  hexasc:
	ble	$a0,9,lessthan
	nop
	addi	$a0,$a0,0x37
	move	$v0,$a0
	j	return
	nop

lessthan:
	addi	$a0,$a0,0x30
	move	$v0,$a0	
return:
	jr 	$ra
	nop
.global delay
delay: 			#whle-loop
	sub 	$a0, $a0, 1   #otherwise, subtract a0 with 1
	beq	$a0, $0, return
	nop
	li	$s6, 0   #i = 0
	li	$s5, 3800   #set constant 4711 to s5.
	
for:	
	
	slt	$t1, $s6, $s5    #if 4711 < 0, set t1 to 1, otherwise set t1 to zero. 
	beq	$t1, $0, delay  #branch to jump if t1 is equal to zero,
	nop
	addi	$s6, $s6, 1  #increase i by 1
	j	for
	nop	

	
				
	
.global time2string
time2string: 
	PUSH $s0
 	PUSH $s1
 	PUSH $ra
 	
 	move $s0,$a0	 #flytta a0 till s0 s� man kan anv�nda a0
 	move $s1,$a1	 #flytta a1 till s1 s� man kan anv�nda a1	
 	

 	
 	# ett
 	andi $a0,$a1,0xf000  #nolla alla bitar f�rutom 12-15
 	srl $a0, $a0,0xc     #shifta bitarna till "least significant bits" och spara i a0
 	jal hexasc		#hoppa till hexasc sub-funktionen
 	nop	     
 	sb $v0, 0($s0)	     #spara biten
 	
 	# tv�
 	andi $a0,$s1,0x0f00  #nolla alla bitar f�rutom fr�n 8-11
 	srl $a0, $a0,0x8     #shifta bitarna 12 till h�ger och spara i a0
 	jal hexasc		#hoppa till hexasc sub-funktionen
 	nop	     
 	sb $v0, 1($s0)	     #spara biten
 	
 	# colon
 	li $v0, 0x3A
 	sb $v0, 2($s0)
 	

 	
 	# tre
 	andi $a0,$a1,0x00f0  #nolla alla bitar f�rutom 4-7
 	srl $a0, $a0,0x4     #shifta bitarna 8 till h�ger och spara i a0
 	jal hexasc	#hoppa till hexasc sub-funktionen, anv�nder $s f�r $t wipas n�r man utf�r en jump operand. 
 	nop     
 	sb $v0, 3($s0)	     #spara biten
 	
 	# fyra
 	andi $a0,$a1,0x000f  #nolla alla bitar f�rutom 0-3
 	srl $a0, $a0,0x0     #shifta bitarna 4 till h�ger och spara i a0
 	jal hexasc		#hoppa till hexasc sub-funktionen
 	nop     
 	sb $v0, 4($s0)	     #spara biten
 	
 	# null byte
        li $a0,0x0
        sb $a0, 5($s0)
        
         POP $ra
   	 POP $s1
         POP $s0

  	 jr  $ra
   	 nop

	 #WRITTEN BY ERIK LOSIN
.global enable_interrupt
enable_interrupt:
	ei
	jr $ra
 
.global disable_interrupt
disable_interrupt: 
	di
	jr $ra