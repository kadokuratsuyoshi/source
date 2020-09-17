.include "1200def.inc"
.def temp = r16
	ldi	temp, 0xff
	out	ddrb, temp
	ldi temp, 0x00
	out	portb, temp
