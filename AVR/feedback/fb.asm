;
;	progname:	fb.asm
;	contents:	a/d and timer interrupt for feedback system
;	make:		avrasm32.exe -fl fb.asm -l fb.lst
;	device:		AT90S1200 (ATMEL)
;
;           /RESET VCC-----------------VCC
;           PD0    PB7(SCK)
;           PD1    PB6(MISO)
; 10MHz-----XTAL1  PB5(MOSI)
;   +-------XTAL2  PB4
;     (INT0)PD2    PB3      FET              VCC
;           PD3    PB2 ----->|-----GND        |         VCC
;       (T0)PD4    PB1(AN1)--(comparator -)--VR1---GND   |
;           PD5    PB0(AN0)--(comparator +)-------------VR2---GND
; GND-------GND    PD6      
;
;
.include "1200def.inc"
.def	temp = r16
.def	data = r17
.def	a = r18
.def	b = r19
.def	c = r20

.org 0000
		rjmp	reset           ; reset
.org 0001
		reti                  ; ext_int0
.org 0002
		rjmp  tim0_ovf        ; tim0_ovf
.org 0003
		rjmp	ana_comp        ; ana_comp

reset:
		; PORTB
		sbi		DDRB, 2 ;DDRB, 5
		; analog comparator
		ldi		temp, (1<<ACIE)
		out		ACSR, temp
		; timer
		ldi		a, $05
		out		TCCR0, a
		ldi		a, $02
		out		TIMSK, a
		ldi		a, $10
		out		TCNT0, a
		; enable interrupt
		sei
main:
		nop
		rjmp	main

tim0_ovf:
		sbrs	c, 0
		reti
		inc		b
		out		PORTB, b
		ldi		a, $10
		out		TCNT0, a
		reti

ana_comp:
		cli
		in		temp, ACSR
		sbrs	temp, ACO
		ldi		c, 1
		sbrc	temp, ACO
		ldi		c, 0 
		ldi		b, 0
		out		PORTB, b	; FET off
		sei
		reti
;		end
