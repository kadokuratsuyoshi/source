#=
	program:	euler.jl
	contents:	numerical solution of m(d^2/dt^2)x=-kx-av by Euler method
=#
function euler()
	x = 1.0
	v = 0.0
	t = 0.0
	dt = 0.01
	k = 1.0
	m = 1.0
	gamma = 1.0
	n = 0
	step = 1
	for n in (0:step:1000)
		println("$t $x $v")
		dx = v * dt
		dv = ((-k*x - gamma*v)/m) * dt
		x = x + dx
		v = v + dv
		t = t + dt
	end
end
euler()
