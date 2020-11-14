;
;	program:	crt0.asm
;	contents:	crt0 stands for the C runtime at the very begining
;	make:	  sdasz80 -a -o -l crt0.asm
;	usage:		sdcc foo.c -mz80 --code-loc 0x0100 --no-std-crt0 -Wlcrt0.o
;
	.module crt0
	.globl	_main
	.area	_HEADER (ABS)

	.org	0
	jp	reset
	.org	0x08
	reti
	.org	0x10
	reti
	.org	0x18
	reti
	.org	0x20
	reti
	.org	0x28
	reti
	.org	0x30
	reti
	.org	0x38
	reti

	.org	0x100
reset:
	ld	sp,#0xffff
	call	gsinit
	call	_main
	jp	_exit

	.area	_HOME
	.area	_CODE
	.area	_GSINIT
	.area	_GSFINAL
	.area	_DATA
	.area	_BSS
	.area	_HEAP

	.area	_CODE
__clock:
	ld	a,#2
	rst	0x08
	ret
_exit:
	ld	a,#0
	rst	0x08
1$:
	halt
	jr	1$

	.area	_GSINIT
gsinit:
	.area	_GSFINAL
	ret
