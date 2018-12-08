	.globl main
	.extern printf
	.text
output_str:
	.ascii "ans: %d\n"
	.align 4

main:
	stmfd r13!, {ip, lr}
	mov r4 ,#0
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	cmp r4, #0
	blne if01
	mov r4 ,#2
	stmfd r13!, {r4}
	bl if01_end
if01:
	mov r4 ,#23
	stmfd r13!, {r4}
if01_end:
	ldmfd r13!, {r1}
	ldr r0, =output_str
	bl printf
	mov r0, #0
	ldmfd sp!, {ip, pc}

