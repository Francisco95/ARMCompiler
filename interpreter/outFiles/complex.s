	.globl main
	.extern printf
	.text
output_str:
	.ascii "ans: %d\n"
	.align 4
fun2:
	stmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
	stmfd r13!, {r0}
	ldmfd r13!, {r4}
	cmp r4, #0
	blne if03
	mov r4 ,#5
	stmfd r13!, {r4}
	bl if03_end
if03:
	mov r4 ,#7
	stmfd r13!, {r4}
if03_end:
	ldmfd r13!, {r0}
	ldmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
fun5:
	stmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
	stmfd r13!, {r0}
	mov r4 ,#2
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	sub r5, r4, r5
	stmfd r13!, {r5}
	ldmfd r13!, {r0}
	ldmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
fun4:
	stmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
	stmfd r13!, {r0}
	mov r4 ,#6
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	add r5, r4, r5
	stmfd r13!, {r5}
	ldr r4, =fun5
	stmfd r13!, {r4}
	mov r4 ,#1
	stmfd r13!, {r4}
	ldmfd r13!, {r0}
	ldmfd r13!, {r4}
	blx r4
	stmfd r13!, {r0}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	add r5, r4, r5
	stmfd r13!, {r5}
	ldmfd r13!, {r0}
	ldmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}

main:
	stmfd r13!, {ip, lr}
	mov r4 ,#3
	stmfd r13!, {r4}
	mov r4 ,#0
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	cmp r4, #0
	blne if01
	ldr r4, =fun2
	stmfd r13!, {r4}
	mov r4 ,#18
	stmfd r13!, {r4}
	ldmfd r13!, {r0}
	ldmfd r13!, {r4}
	blx r4
	stmfd r13!, {r0}
	bl if01_end
if01:
	mov r4 ,#3
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	sub r5, r4, r5
	stmfd r13!, {r5}
if01_end:
	ldr r4, =fun4
	stmfd r13!, {r4}
	mov r4 ,#7
	stmfd r13!, {r4}
	ldmfd r13!, {r0}
	ldmfd r13!, {r4}
	blx r4
	stmfd r13!, {r0}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	sub r5, r4, r5
	stmfd r13!, {r5}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	add r5, r4, r5
	stmfd r13!, {r5}
	ldmfd r13!, {r1}
	ldr r0, =output_str
	bl printf
	mov r0, #0
	ldmfd sp!, {ip, pc}

