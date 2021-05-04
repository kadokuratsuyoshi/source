	nam	piacount
* PIA test program
pia	equ	$a00000
*
	org	$1000
	lea	pia,a0
	moveq	#0,d0
	movep.l	d0,0(a0)
	move.l	#$ff3cff3c,d0
	movep.l	d0,0(a0)
	moveq	#0,d0
loop	moveq	#0,d1
delay	addq.w	#1,d1
	bne	delay
	addq	#1,d0
	move.b	d0,(a0)
	ror	#8,d0
	move.b	d0,4(a0)
	rol	#8,d0
	bra	loop
	end
