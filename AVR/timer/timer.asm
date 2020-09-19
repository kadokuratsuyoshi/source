;
;	progname:	led.asm
;	contents:	led blinking
;	make:		avrasm32.exe -fl led.asm -l led.lst
;
;           /RESET VCC-----------------VCC
;           PD0    PB7(SCK)
;           PD1    PB6(MISO)
; 10MHz-----XTAL1  PB5(MOSI)
;   L-------XTAL2  PB4
;     (INT0)PD2    PB3
;           PD3    PB2
;       (T0)PD4    PB1(AN1)
;           PD5    PB0(AN0)----->|-----GND
; GND-------GND    PD6          LED
;
;
.include "1200def.inc"
.def	a = r16
.def	b = r17

.org 0000
		rjmp	reset			; reset
.org 0001
		reti					; ext_int0
.org 0002
		rjmp	tim0_ovf 		; tim0_ovf
.org 0003
		reti					; ana_comp

reset:
		sbi		DDRB, 0
		ldi		a, $05
		out		TCCR0, a
		ldi		a, $02
		out		TIMSK, a
		ldi		a, $0 ;$FF
		out		TCNT0, a
		sei
main:
		nop
		rjmp	main

tim0_ovf:
		inc		b
		out		PORTB, b
		ldi		a, $0 ;$FF
		out		TCNT0, a
		reti
;		end
