	.globl main
	.extern printf
	.text
output_str:
	.ascii "ans: %d\n"
	.align 4

main:
	stmfd r13!, {ip, lr}
	mov r4 ,#12
	stmfd r13!, {r4}
	mov r4 ,#24
	stmfd r13!, {r4}
	ldmfd r13!, {r4}
	ldmfd r13!, {r5}
	sub r5, r4, r5
	stmfd r13!, {r5}
	ldmfd r13!, {r1}
	ldr r0, =output_str
	bl printf
	mov r0, #0
	ldmfd sp!, {ip, pc}

