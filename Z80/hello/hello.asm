	org	100h
start:
	mvi		c, 09h
	lxi		d, text
	call	0005h
	ret

test:
	db		'hello, world', 0dh, 0ah, '$'
	end
