/*
	filename:	hello.s
	contents:	hello, world
	make:		as hello.s
	usage:		./hello
*/
	.data
msg:
	.asciz		"hello, world"

	.balign		4
return:
	.word		0

	.text
	.global		main
main:
	ldr	r1, =return		@ r1 <- &return
	str	lr, [r1]		@ *r1 <- lr

	ldr	r0, =msg		@ r0 <- &msg
	bl	puts			@ Call puts

	ldr	r1, =return		@ r1 <- &return
	ldr	lr, [r1]		@ lr <- *r1
	bx	lr				@ return from main

/* External */
	.global puts
; eof
