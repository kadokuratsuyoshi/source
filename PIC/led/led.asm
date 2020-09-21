;
;	progname:	led.asm
;	contents:	led blink
;	make:		gpasm -w2 led.asm
;	device:		PIC16F873
;
;         LED    1/MCLR   RB7
;   GND---|<---- RA0/AN0  RB6 
;                RA1/AN1  RB5 
;                RA2      RB4
;                RA3      RB3 
;                RA4      RB2 
;                RA5      RB1
;   GND----------VSS  RB0/INT
;    +-----------CLKIN    VDD------+5V
;   4MHz---------CLKOUT   VSS------GND
;                RC0      RC7 
;                RC1      RC6
;                RC2      RC5
;                RC3      RC4 
;
;	
	LIST	P=PIC16F873
	INCLUDE "P16F873.INC"

	__config _CP_OFF & _HS_OSC & _WDT_OFF & _PWRTE_ON & _LVE_OFF & _BODEN_OFF

	CBLOCK	0x20
	loop1,loop2
	ENDC

    	ORG	00H
	GOTO	reset
	ORG	04H
	GOTO	intr

reset:
	BSF	STATUS, RP0		; RAM Bank 1
	CLRF	TRISA			; All pins Port A output
	CLRF	TRISB			; All pins Port B output
	CLRF	TRISC			; All pins Port C output
	MOVLW	06H
	MOVWF	ADCON1			; ALL pins digital I/O
	BCF	STATUS, RP0		; RAM Bank 1
	CLRF	PORTA			; Initialize Port A
	CLRF	PORTB			; Initialize Port B
	CLRF	PORTC			; Initialize Port C

main:
	BSF	PORTA, 0		; LED on
	CALL	delay
	BCF	PORTA, 0		; LED off
	CALL	delay
	GOTO	main

delay:
	MOVLW	D'128'
	MOVWF	loop1
lp1:
	MOVLW	D'128'
	MOVWF	loop2
lp2:
	NOP
	DECFSZ	loop2, F
	GOTO	lp2
	DECFSZ	loop1, F
	GOTO	lp1
	RETURN

intr:
	RETFIE
	END
