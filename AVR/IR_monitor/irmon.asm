;
;	progname:	remmon.asm
;	contents:	IR remote controller monitor using by PhotoDiode(PD)
;	make:		avrasm32 -fI remmon.asm -l remmon.lst
;	CPU:		AT90S1200
;	usage:
;
;       RESET   VCC------+3V
;       PD0     PB7
;       PD1     PB6
;10MHz--XTAL2   PB5
;  |----XTAL1   PB4                           +3V
;       PD2     PB3                           |
;       PD3     PB2---->|(led)---GND          |
;       PD4     PB1-------------------------->VR10k
;       PD5     PB0---->|(PD)----GND          |
;GND----GND     PD6                           GND
;
;
;
.include "1200def.inc"
.def	a = R16

reset:
		ldi		a, 0b1111_1100
		out		DDRB, a
		ldi		a, 0b0000_0000
		out		PORTB, a

main:
		sbic	0x08, 5
		rjmp	on
off:
		cbi		PORTB, 2
		rjmp	main
on:
		sbi		PORTB, 2
		rjmp	main
;
;		eof
