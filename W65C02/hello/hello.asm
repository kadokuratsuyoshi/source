;
;	program:	hello.asm
;	contents:	display hello world on W65C02 Apple 1 Replica
;	make:		>msdos x6502 /H hello.asm,hello.HEX,hello.LST,hello.CRF,hello.MAP
;	usage:		R6000
;
				.ORG	$6000
PRBYTE			.EQU	$FFDC
ECHO			.EQU	$FFEF
CR              .EQU    $8D             Carriage Return

START
				JSR		HELLO
				JSR		COUNT
				RTS

HELLO
				LDX		#0
L1
				LDA		L3,X
				BPL		L2
				JSR		ECHO
				INX
				BNE		L1
L2
				ORA		#$80	;%1000.0000
				JMP		ECHO
L3
				.byte	$48,$45,$4c,$4c,$4e,$20,$57,$4e,$53,$4e,$44 ; HELLO WORLD

COUNT
				JSR		L22
				LDX		#0
L21
				TXA
				JSR		PRBYTE
				LDA		#" "
				JSR		ECHO
				INX
				CPX		#10
				BCC		L21
L22
				LDA		#CR
				JMP		ECHO
; eof
