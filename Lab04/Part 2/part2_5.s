
.data
.balign 4

string1: .asciz "\n Give me the value of x:"
string2: .asciz "\n 6*%d^2 +9*%d +2 = %d\n"
string3: .asciz "\n x= %d"
format:  .asciz " %d"


a:      .int 0
b:	.int 0
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
        
        @aquí la operación
        ldr r1, = a
        ldr r1, [r1]
        
        MUL r2, r1, r1
        ldr r7, =b
        str r2, [r7]
        
        ldr r4, =c
        ldr r4, [r4]
        add r5, r4, #6
        ldr r3, = c
        str r5, [r3]
        
        ldr r4,=b
        ldr r4, [r4]
        ldr r3,=c
        ldr r3, [r3]
        MUL r7, r4, r3
        ldr r2, =c
        str r7, [r2]
        
        @Hasta aquí, c = 6x2
        
        ldr r4,=b
        mvn r4,#0x0
        add r5, r4, #0xA
        ldr r3, = b
        str r5, [r3]
        
        ldr r1, =a
        ldr r1, [r1]
        ldr r4, =b
        ldr r4, [r4]
        MUL r7, r1, r4
        ldr r3, =b
        str r7, [r3]
        
        @Hasta aquí, c = 6x2 y b = 9x
        
        ldr r4, =b
        ldr r4, [r4]
        ldr r3, =c
        ldr r3, [r3]
        add r5, r4, r3
        ldr r3, = b
        str r5, [r3]
        ldr r3, = b
        ldr r3,[r3]
        add r5, r3, #2
        ldr r4, = c
        str r5, [r4]
        
        ldr r1, =a
        ldr r1, [r1]
        ldr r2, =a
        ldr r2, [r2]
        ldr r3, =c
        ldr r3, [r3]
        ldr r0, =string2
        bl 	printf
        
        pop  {ip, pc}
