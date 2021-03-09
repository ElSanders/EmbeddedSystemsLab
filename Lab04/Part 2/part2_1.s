
.data
string1: .asciz "\n%d\n"
string2: .asciz "\n%x\n"
y:	.word 33
f:      .word 0xAAAAAAAA
g:      .word 0xBBBBBBBB
h:      .word 0xCCCCCCCC
	
.text
.global main
.extern printf

main:
        push {ip, lr}
        ldr  r0, =string2
        ldr  r1, =y
        bl   printf
        
        ldr  r0, =string1
        ldr  r1, =y
        ldr  r1, [r1]  
        bl   printf
        
        ldr  r2, =f 
        ldr  r2, [r2]
        ldr  r1, =y
        str  r2, [r1]
        ldr  r0, =string2
        ldr  r1,=y
        ldr  r1, [r1]
        bl   printf
        
        ldr  r2, =g
        ldr  r2, [r2]
        ldr  r1,=y
        str  r2, [r1,#0]
        ldr  r0, =string2
        ldr  r1, =y
        ldr  r1, [r1]  
        bl   printf
        
        ldr  r2, =h
        ldr  r2, [r2]
        ldr  r1,=y
        str  r2, [r1], #0
        ldr  r0, =string2
        ldr  r1, =y
        ldr  r1, [r1]  
        bl   printf
        
        pop  {ip, pc}
