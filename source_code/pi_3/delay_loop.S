	.text

	.align	6

	.globl	delay_loop
delay_loop:
	subs	w0, w0, #1
	bne	delay_loop
	ret
	