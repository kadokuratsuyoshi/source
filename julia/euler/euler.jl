#=
	program:	euler.jl
	contents:	numerical solution of m(d^2/dt^2)x=-kx-av by Euler method
=#
function euler()
	t = 0.0
	dt = 0.01
	x = 1.0
	v = 0.0
	k = 1.0
	m = 1.0
	a = 1.0
	for t in (0:dt:10)
		println("$t $x $v")
		dx = v * dt
		dv = (-1.0*(k/m)*x - (a/m)*v) * dt
		x = x + dx
		v = v + dv
	end
end
euler()
