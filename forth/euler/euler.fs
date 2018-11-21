: euler
	0.0 t !
	0.01 dt !
	1.0  x !
	0.0  v !
	1.0  k !
	1.0  m !
	1.0  a !
	cr 1000 0 do cr i.
		(v dt *.) to dx
		((((k m / .) x *.) -1.0 * .) ((a m / .) dt *.) -.) to dv
		(x dx + .) to x
		(v dv + .) to v
		(t dt + .)
		cr t . x . v .
	loop;

