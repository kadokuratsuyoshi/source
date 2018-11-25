: euler
	fvariable t 0.0e t f!
	fvariable dt 0.01e dt f!
	fvariable x 1.0e x f!
	fvariable v 0.0e v f!
	fvariable k 1.0e k f!
	fvariable m 1.0e m f!
	fvariable a 1.0e a f!
	fvariable p 0.0e p f!
	fvariable q 0.0e q f!
	cr 100 0 do cr i.
		v f@ dt f@ f* dx f!
		-1.0e k f@ f* m f@ f/ x f@ f* p f!
		a f@  m f@ f/ v f@ f* q f!
		p f@  q f@ f- dt f@ f* dv f!
		x f@ dx f@ f+ x f!
		v f@ dv f@ f+ v f!
		t f@ dt f@ f+ t f!
		cr t f@ t f. x f@ x f. v f@ x f.
	loop;
