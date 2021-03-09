
.data
.balign 4

string1: .asciz "\n Give me the first operand: "
string2: .asciz "\n Give me the second operand: "
string3: .asciz "\n %d %c %d "
string4: .asciz "\n Give me the operation: "
string5: .asciz "\n Give me the third operand: "
format: .asciz "%d"
output: .asciz  "%c %d = %d\n"
sign: .asciz " %c"


a:  .int 0
b:	.int 0
c:  .int 0
d:	.int 0
s:  .word 44
s2: .word 55


.text
.global main
.extern printf
.extern scanf

main:
        push {ip, lr}
        ldr  r0, =string1
        bl   printf
        
        ldr r0, =format
        ldr r1, =a
        bl scanf

        ldr r0, =string4
        bl printf
        
        ldr r0, =sign 
        ldr r1, =s
        bl scanf
        
        ldr  r0, =string2
        bl   printf
        
        ldr r0, =format
        ldr r1, =b
        bl scanf
        
        ldr r0, =string4
        bl printf
        
        ldr r0, =sign 
        ldr r1, =s2
        bl scanf
        
        ldr  r0, =string5
        bl   printf
        
        ldr r0, =format
        ldr r1, =c
        bl scanf
        
        ldr r1, =a
        ldr r1, [r1]
        
        ldr r2, =b
        ldr r2, [r2]
        
        ldr r3, =s
        ldr r3, [r3]
        
        ldr r4, =c
        ldr r4, [r4]
        
        ldr r5, =s2
        ldr r5, [r5]       
        
        
        cmp r3,#43 @if char == +
        beq suma
        cmp r3,#45 @if char == -
        beq resta
        cmp r3,#42 @if char == *
        beq multi
        cmp r3,#47 @if char == /
        beq divis
        
     suma: 
        add r3, r1, r2  @suma
        ldr r6, =d
        str r3, [r6]
        b next
    resta: 
        sub r3, r1, r2  @resta
        ldr r6, =d
        str r3, [r6]
        b next
    multi: 
        mul r3, r1, r2  @multiplicacion
        ldr r6, =d
        str r3, [r6]
        b next
    divis: 
        sdiv r3, r1, r2  @division
        ldr r6, =d
        str r3, [r6]
        b next
    next:
        cmp r5,#43 @if char == +
        beq suma2
        cmp r5,#45 @if char == -
        beq resta2
        cmp r5,#42 @if char == *
        beq multi2
        cmp r5,#47 @if char == /
        beq divis2
        
     suma2:
		ldr r6, =d
		ldr r6, [r6]
        add r3, r6, r4  @suma
        ldr r4, =d
        str r3, [r4]
        b prints
    resta2: 
		ldr r6, =d
		ldr r6, [r6]
        sub r3, r6, r4  @suma
        ldr r4, =d
        str r3, [r4]
        b prints
    multi2:
		ldr r6, =d
		ldr r6, [r6]
        mul r3, r6, r4  @suma
        ldr r4, =d
        str r3, [r4]
        b prints
    divis2:
		ldr r6, =d
		ldr r6, [r6]
        sdiv r3, r6, r4  @suma
        ldr r4, =d
        str r3, [r4]
        b prints
        
	prints:        
        ldr	r1, =a           @ r1 <- a
        ldr	r1, [r1]
        ldr r2, =s			 @ r2 <- sign
        ldr r2, [r2]
        ldr	r3, =b           @ r3 <- b
        ldr	r3, [r3]
        ldr  r0, =string3 
        bl   printf
        
        ldr r0,=output
        ldr	r1, =s2           @ r1 <- s2
        ldr	r1, [r1]
        ldr	r2, =c           @ r2 <- c
        ldr	r2, [r2]
        ldr	r3, =d           @ r3 <- d
        ldr	r3, [r3]
        bl printf
        
        
        
        pop  {ip, pc}
