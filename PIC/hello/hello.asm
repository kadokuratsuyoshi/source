	LIST	P=PIC16F84
	INCLUDE	"P16F84.INC"

BITC	EQU	0CH
CNT1	EQU	0DH
CNT2	EQU	0EH
CNT3	EQU	0FH
TEMP	EQU	010H

	ORG	0
	GOTO	MAIN
	ORG	4
	GOTO	INTR
MAIN
	BSF	STATUS, RP0
	CLRF	TRISB
	BSF	TRISB, 0

	CLRF	TRISA
	BCF	STATUS, RP0

	MOVLW	B'10010000'
	MOVWF	INTCON
	GOTO	IDLE
INTR
	BCF	INTCON, 1
	CLRF	PORTA
	MOVLW	B'10101010'
	MOVWF	PORTA
	RETFIE
IDLE
	SLEEP
	END
