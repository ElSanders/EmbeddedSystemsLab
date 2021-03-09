
.data
.balign 4

string1: .asciz "\n Give me the first operand: "
string2: .asciz "\n Give me the second operand: "
string3: .asciz "\n %d %c %d = "
string4: .asciz "\n Give me the operation: "
format: .asciz "%d"
output: .asciz  "%d\n"
sign: .asciz " %c"


a:  .int 22
b:	.int 33
s:  .word 44
c:	.int 0

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
        
        
        
       
        
        ldr r1, =a
        ldr r1, [r1]
        
        ldr r2, =b
        ldr r2, [r2]
        
        ldr r3, =s
        ldr r3, [r3]       
        
        @sub r5, r1, r2  @resta
        @MUL r6, r1, r2  @multiplicacion
        @sdiv r7, r1, r2  @division
        
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
        ldr r4, =c
        str r3, [r4]
        b prints
    resta: 
        sub r3, r1, r2  @suma
        ldr r4, =c
        str r3, [r4]
        b prints
    multi: 
        mul r3, r1, r2  @suma
        ldr r4, =c
        str r3, [r4]
        b prints
    divis: 
        sdiv r3, r1, r2  @suma
        ldr r4, =c
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
        ldr	r1, =c           @ r1 <- c
        ldr	r1, [r1]
        bl printf
        
        
        
        pop  {ip, pc}
