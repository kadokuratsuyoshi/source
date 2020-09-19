;
;	progname:	ad.asm
;	contents:	a/d interrupt for led blinking
;	make:		avrasm32.exe -fl ad.asm -l ad.lst
;
;           /RESET VCC-----------------VCC
;           PD0    PB7(SCK)
;           PD1    PB6(MISO)    LED
; 10MHz-----XTAL1  PB5(MOSI)---->|-----GND
;   +-------XTAL2  PB4
;     (INT0)PD2    PB3                       VCC
;           PD3    PB2                        |         VCC
;       (T0)PD4    PB1(AN1)--(comparator -)--VR1---GND   |
;           PD5    PB0(AN0)--(comparator +)-------------VR2---GND
; GND-------GND    PD6      
;
;
.include "1200def.inc"
.def	temp = r16
.def	data = r17

.org 0000
		rjmp	reset           ; reset
.org 0001
		reti                    ; ext_int0
.org 0002
		reti                    ; tim0_ovf
.org 0003
		rjmp	ana_comp        ; ana_comp

reset:
		sbi	DDRB, 5
		ldi	temp, (1<<ACIE)
		out	ACSR, temp
		sei
main:
		nop
		rjmp	main

ana_comp:
		cli
		in	temp, ACSR
		sbrs	temp, ACO
		sbi	PORTB, 5
		sbrc	temp, ACO
		cbi	PORTB, 5
		sei
		reti
;		end
