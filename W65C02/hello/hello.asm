		.org	$0280
Echo	.equ	$ffef
Monitor	.equ	$ff1f

Begin:
		LDX		#$00
Print:
		LDA		String,X
		CMP		#$00
		BEQ		Done
		JSR		Echo
		INX	
		JMP		Print
Done:
		JMP		Monitor
String:
		.asc	"HELLO, WORLD",$00
