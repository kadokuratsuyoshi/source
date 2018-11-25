\ euler.fs, 79-FORTH ROM for Apple-1(L-Star/Propeller Computer) or gforth, DoKA
fvariable t
fvariable dt
fvariable x
fvariable v
fvariable dx
fvariable dv
fvariable k
fvariable m
fvariable a
fvariable p
fvariable q
: euler
	0.0e t f!
	0.01e dt f!
	1.0e x f!
	0.0e v f!
	0.0e dx f!
	0.0e dv f!
	1.0e k f!
	1.0e m f!
	1.0e a f!
	0.0e p f!
    	0.0e q f!
	1000 0 do 
		v f@ dt f@ f* dx f!
		-1.0e k f@ f* m f@ f/ x f@ f* p f!
		a f@  m f@ f/ v f@ f* q f!
		p f@  q f@ f- dt f@ f* dv f!
		x f@ dx f@ f+ x f!
		v f@ dv f@ f+ v f!
		t f@ dt f@ f+ t f!
		cr t f@ t f. x f@ x f. v f@ x f.
	loop
;
