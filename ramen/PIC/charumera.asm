;
;	charumera.asm
;	Copyright (C) 2003 by kado otherwise DoKA
;
;	>pa -l charumera.asm
;
;	device PIC16F84 10.000MHz
;
	include 16f84.h
	.osc	xt
	.pwrt	on
	.wdt	off
	.protect off

mml_table_a_max	equ	25

;mml_table_a_max	equ	240
;mml_table_b_max	equ	251

sound_out_port	equ	rb.0

def_o_data	equ	'1'
def_l_data	equ	'5'
def_t_data	equ	'4'

	org	0ch
work		ds	3
flug_set	ds	1
mml_cmd		ds	1
mml_cmd_new	ds	1
mml_cmd_old	ds	1
mml_l_data	ds	1
mml_o_data	ds	1
mml_ml_data	ds	1
mml_f_data	ds	1
mml_t_data	ds	1
tmr0_int_cnt	ds	1	
tmr0_int_cmp	ds	1
music_table_cnt	ds	1
s_l_num		ds	1
s_f_num		ds	1
f_h_num		ds	1
f_l_num		ds	1
half_up_flug	equ	flug_set.0
half_down_flug	equ	flug_set.1

	org	0
	jmp	start
	org	4
int:	bcf	gie
	bcf	t0if
	inc	tmr0_int_cnt	
	retfie	

;--------------------
;  mml_command_data
;--------------------
music_table_a:
	clr	pclath
	mov	w,work[0]
	jmp	pc+w
	retw	't4o2l3cde7dcrrrcdedcd7rrrr'

;	retw	't4o2l3gggg1g5r5ggga1g5rcgggggd6'		;music_table_a
;	retw	'e5f5e5dc6r6>a<ed7rc>b<c7r7'			;240byte
;	retw	'gggg1g5rcggga1g5rc'
;	retw	'gggggdd5e5f5e5dc6r5ced1d7'
;	retw	'rc>b<c7r7dddre'
;	retw	'eeecrccca5a5gfeg8r5dddd5d'
;	retw	'ga5g6rc1d1c5r>a1b1<c5>a<fedc>b<cd9'
;	retw	'r5c2c2c2cc1c1cd1c5d1d1dddde1d5'
;	retw	'l1eee3eee3eee6eee3eee3eee5l3cc5e5d5c7'

	org	100h
music_table_b:
	mov	pclath,#1
	mov	w,work[0]
	jmp	pc+w
	retw	'r>g<eefe6r>g<eefg6r>g<ddec9r9'			;music_table_b
	retw	'f<cc6>'					;251byte
	retw	'b5ga5aaga5<c5>b<cde8r>g1g1'
	retw	'<c5>ra1b1<c5dddc>b<cc5>ra1b1<c5>ra1b1<c5r5'
	retw	'l1ddd3d3d3d3ed4>g3<ddd3d2d2d2dded5r9'
	retw	'l2cccl3cc1c1cd1c5d1d1dddde1d5e1e1eeeef1e5'
	retw	'ee1e1e5ef1e4cc5e5dc>b<c6>rg<eefe6'
	retw	'r>g<eefg6r>g<ddec9r>g<ddec9'
	retw	'>>r6g<ddec9'

	org	200h
start:
	call	sys_init

main_loop:
	cje	ra,#0001b,main_loop

	;--------------------
	; mml_command read
	;--------------------
		;----------------------
		; music_table_a read
		;----------------------
		clr	music_table_cnt
mml_read_a_loop:
		mov	work[0],music_table_cnt
		call	music_table_a		
		mov	mml_cmd,w
		call	mml	
		inc	music_table_cnt
		cjb	music_table_cnt,#mml_table_a_max+1,mml_read_a_loop

		;----------------------
		;  music_table_b read
		;----------------------
;		clr	music_table_cnt
;mml_read_b_loop:
;		mov	work[0],music_table_cnt
;		call	music_table_b
;		mov	mml_cmd,w		
;		call	mml		
;		inc	music_table_cnt
;		cjb	music_table_cnt,#mml_table_b_max+1,mml_read_b_loop

	jmp	main_loop


sys_init:
	bsf	rp0

	mov	!ra,#00001b	;ra

;	clr	ra
	clr	rb
	bcf	rp0

	clr	ra
	clr	rb

	clr	flug_set
	clr	mml_cmd_old
	mov	mml_o_data,#def_o_data
	mov	mml_ml_data,#def_l_data
	mov	mml_t_data,#def_t_data	

	bsf	rp0
;	mov	option,#11010101b	;tmr0 1/64	X'tal 3.58MHz
	mov	option,#11010110b	;tmr0 1/128	X'tal 10.00MHz
	bcf	rp0
	bcf	t0if
	bsf	t0ie

	ret

;  mml_cmd <- MML_command(1byte)
;
mml:
	mov	mml_cmd_old,mml_cmd_new
	mov	mml_cmd_new,mml_cmd

	cjb	mml_cmd_new,#'0',mml_st_cmd
	cja	mml_cmd_new,#'9',mml_st_cmd

mml_st_fig:
		cje	mml_cmd_old,#'r',mml_l_set
		cjb	mml_cmd_old,#'a',mml_l_set_skip
		cja	mml_cmd_old,#'g',mml_l_set_skip
mml_l_set:	mov	mml_l_data,mml_cmd_new
		call	sound_out
		jmp	mml_end
mml_l_set_skip:

		cjne	mml_cmd_old,#'o',mml_o_set_skip
		mov	mml_o_data,mml_cmd_new
		jmp	mml_end
mml_o_set_skip:

		cjne	mml_cmd_old,#'l',mml_ml_set_skip
		mov	mml_ml_data,mml_cmd_new
		jmp	mml_end
mml_ml_set_skip:

		cjne	mml_cmd_old,#'t',mml_t_set_skip
		mov	mml_t_data,mml_cmd_new
;		jmp	mml_end
mml_t_set_skip:
		jmp	mml_end

mml_st_cmd:
	cje	mml_cmd_old,#'r',mml_old_dec
	cjb	mml_cmd_old,#'a',mml_old_dec_skip
	cja	mml_cmd_old,#'g',mml_old_dec_skip
mml_old_dec:
	mov	mml_l_data,mml_ml_data
	call	sound_out
mml_old_dec_skip:

		cje	mml_cmd_new,#'r',mml_f_set
		cjb	mml_cmd_new,#'a',mml_f_set_skip
		cja	mml_cmd_new,#'g',mml_f_set_skip
mml_f_set:	mov	mml_f_data,mml_cmd_new
		jmp	mml_end
mml_f_set_skip:

		cjne	mml_cmd_new,#'<',mml_o_inc_skip
		inc	mml_o_data
		jmp	mml_end
mml_o_inc_skip:

		cjne	mml_cmd_new,#'>',mml_o_dec_skip
		dec	mml_o_data
		jmp	mml_end
mml_o_dec_skip:

		cjne	mml_cmd_new,#'+',mml_half_up_skip
		bsf	half_up_flug
		jmp	mml_end
mml_half_up_skip:

		cjne	mml_cmd_new,#'-',mml_half_down_skip
		bsf	half_down_flug
;		jmp	mml_end
mml_half_down_skip:
mml_end:
	ret

sound_out:
		mov	work[0],mml_l_data
		sub	work[0],#'0'	

		call	sound_l_table
		mov	s_l_num,w
		jmp	sound_l_table_skip
sound_l_table:
		mov	pclath,#2
		mov	w,work[0]
		jmp	pc+w
		retw	1,2,3,4,6,8,12,16,24,32
sound_l_table_skip:

		mov	work[0],mml_t_data
		sub	work[0],#'0'

			clr	tmr0_int_cmp
sound_tmr_num_loop:	add	tmr0_int_cmp,s_l_num
			djnz	work[0],sound_tmr_num_loop

		cjne	mml_f_data,#'r',sound_r_skip

sound_r:		clr	tmr0_int_cnt
			clr	tmr0
			bsf	gie
			cjne	tmr0_int_cmp,tmr0_int_cnt,$
			bcf	gie	
			jmp	sound_out_end
sound_r_skip:

		mov	work[0],mml_f_data
		sub	work[0],#'a'

		call	sound_f_cnv
		mov	s_f_num,w
		jmp	sound_f_cnv_skip
sound_f_cnv:
		mov	pclath,#2
		mov	w,work[0]
		jmp	pc+w
		retw	9,11,0,2,4,5,7
sound_f_cnv_skip:

		mov	s_o_num,mml_o_data
		sub	s_o_num,#'0'	

			jnb	half_up_flug,sound_h_up_skip
sound_h_up:		inc	s_f_num
			cjb	s_f_num,#12,sound_h_skip

				clr	s_f_num
				csa	s_o_num,#2
				inc	s_o_num
				jmp	sound_h_skip
sound_h_up_skip:
			jnb	half_down_flug,sound_h_skip
sound_h_down:		dec	s_f_num
			cjb	s_f_num,#12,sound_h_skip

				mov	s_f_num,#11
				cse	s_o_num,#0
				dec	s_o_num
;				jmp	sound_h_skip
sound_h_skip:
		bcf	half_up_flug
		bcf	half_down_flug

			clc
			mov	work[0],s_o_num
			rl	work[0]
			rl	work[0]
			mov	work[1],s_o_num
			rl	work[1]
			rl	work[1]
			rl	work[1]
			add	work[1],work[0]

		mov	work[0],s_f_num
		add	work[0],work[1]
		rl	work[0]			

		call	sound_f_table
		mov	f_h_num,w

		inc	work[0]
		call	sound_f_table
		mov	f_l_num,w

		clr	tmr0_int_cnt
		clr	tmr0	
		bsf	gie	

sound_out_loop:	bsf	sound_out_port
		call	f_wait
		jmp	$+1
		jmp	$+1
		nop	
		bcf	sound_out_port
		call	f_wait

		cjne	tmr0_int_cmp,tmr0_int_cnt,sound_out_loop

		bcf	gie
		call	wait_s

sound_out_end:
	ret

f_wait:
	mov	work[0],f_h_num
	mov	work[1],f_l_num

	cje	work[0],#0,f_wait_h_skip
f_wait_h:
	mov	work[2],#84
	djnz	work[2],$
	djnz	work[0],f_wait_h
f_wait_h_skip:

	djnz	work[1],$

	ret

wait_s:
	clr	work[0]
	djnz	work[0],$
	ret

sound_f_table:
	mov	pclath,#3
	mov	w,work[0]
	jmp	pc+w

	retw	13,26	;o0 c 130.81Hz
	retw	12,47	;   . 138.59Hz
	retw	11,72	;   d 146.83Hz
	retw	11,15	;   . 155.56Hz
	retw	10,47	;   e 164.81Hz
	retw	9,81	;   f 174.61Hz
	retw	9,33	;   . 184.99Hz
	retw	8,73	;   g 195.99Hz
	retw	8,31	;   . 207.65Hz
	retw	7,76	;   a 220.00Hz
	retw	7,38	;   . 233.08Hz
	retw	7,2	;   b 246.94Hz

	retw	6,53	;o1 c 261.62Hz
	retw	6,21	;   . 277.18Hz
	retw	5,76	;   d 293.66Hz
	retw	5,48	;   . 311.12Hz
	retw	5,21	;   e 329.62Hz
	retw	4,81	;   f 349.22Hz
	retw	4,57	;   . 369.99Hz
	retw	4,34	;   g 391.99Hz
	retw	4,13	;   . 415.30Hz
	retw	3,78	;   a 440.00Hz
	retw	3,59	;   . 466.16Hz
	retw	3,41	;   b 493.88Hz

	retw	3,24	;o2 c 523.25Hz
	retw	3,8	;   . 554.36Hz
	retw	2,78	;   d 587.32Hz
	retw	2,64	;   . 622.25Hz
	retw	2,51	;   e 659.25Hz
	retw	2,38	;   f 698.45Hz
	retw	2,26	;   . 739.98Hz
	retw	2,15	;   g 783.99Hz
	retw	2,4	;   . 830.60Hz
	retw	1,79	;   a 880.00Hz
	retw	1,70	;   . 932.32Hz
	retw	1,61	;   b 987.76Hz

	retw	1,52	;o3 c 1046.50Hz
	retw	1,44	;   . 1108.73Hz
	retw	1,37	;   d 1174.65Hz
	retw	1,30	;   . 1244.50Hz
	retw	1,23	;   e 1318.51Hz
	retw	1,16	;   f 1396.91Hz
	retw	1,10	;   . 1479.97Hz
	retw	1,5	;   g 1567.98Hz
	retw	0,84	;   . 1661.21Hz
	retw	0,79	;   a 1760.00Hz
	retw	0,74	;   . 1864.65Hz
	retw	0,70	;   b 1975.53Hz

;	eof
