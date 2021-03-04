\ rk.fs		Runge-Kutta method for FORTH
fvariable t
fvariable x
fvariable v
fvariable dt
fvariable dx
fvariable dv
fvariable k
fvariable m
fvariable a 
fvariable ct
fvariable cx
fvariable cv
fvariable xa
fvariable xb
fvariable xc
fvariable xd
fvariable va
fvariable vb
fvariable vc
fvariable vd

: rkv { f: t f: x f: v }
	v
;

: rka { f: t f: x f: v }
	-1.0e k f@ f* x f* a f@ v f* f- m f@ f/
;

: rk
	0.0e t f!
	1.0e x f!
	0.0e v f!
	0.01e dt f!
	1.0e k f!
	1.0e m f!
	1.0e a f!
	1000 0 do
		cr t f@ t f. x f@ x f. v f@ v f.

		t f@ x f@ v f@ rkv dt f@ f* xa f! 
		t f@ x f@ v f@ rka dt f@ f* va f!

		dt f@ 2.0e f/ t f@ f+ ct f!
		xa f@ 2.0e f/ x f@ f+ cx f!
		va f@ 2.0e f/ v f@ f+ cv f!
		ct f@ cx f@ cv f@ rkv dt f@ f* xb f!
		ct f@ cx f@ cv f@ rka dt f@ f* vb f!

		dt f@ 2.0e f/ t f@ f+ ct f!
		xb f@ 2.0e f/ x f@ f+ cx f!
		vb f@ 2.0e f/ v f@ f+ cv f!
		ct f@ cx f@ cv f@ rkv dt f@ f* xc f!
		ct f@ cx f@ cv f@ rka dt f@ f* vc f!

		dt f@ t f@ f+ ct f!
		xc f@ x f@ f+ cx f!
		vc f@ v f@ f+ cv f!
		ct f@ cx f@ cv f@ rkv dt f@ f* xd f!
		ct f@ cx f@ cv f@ rka dt f@ f* vd f!

		xa f@ cx f!
		xb f@ 2.0e f* xb f!
		xb f@ cx f@ f+ cx f!
		xc f@ 2.0e f* xc f!
		xc f@ cx f@ f+ cx f!
		xd f@ cx f@ f+ cx f!
		cx f@ 6.0e f/ dx f!

		va f@ cv f!
		vb f@ 2.0e f* vb f!
		vb f@ cv f@ f+ cv f!
		vc f@ 2.0e f* vc f!
		vc f@ cv f@ f+ cv f!
		vd f@ cv f@ f+ cv f!
		cv f@ 6.0e f/ dv f!

		t f@ dt f@ f+ t f!
		x f@ dx f@ f+ x f!
		v f@ dv f@ f+ v f!
	loop
;
