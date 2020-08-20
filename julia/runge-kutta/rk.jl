#=
	program:	rk.jl
	contents:	numerical solution of m(d^2/dt^2)x = -kx -av by Runge-Kutta method
=#
function rk_v(t, x, v)
	return v
end

function rk_a(t, x, v)
	k = 1.0
	m = 1.0
	gamma = 1.0
	return ( (-k*x - gamma*v)/m )
end

function rk()
	dt = 0.01
	t = 0.0
	x = 1.0
	v = 0.0
	for t in (0:dt:10)
		println("$t $x $v")
		k1 = [rk_v(t, x, v)*dt, rk_a(t, x, v)*dt]
		k2 = [rk_v(t+dt/2.0, x+k1[1]/2.0, v+k1[2]/2.0)*dt,
			  rk_a(t+dt/2.0, x+k1[1]/2.0, v+k1[2]/2.0)*dt]
		k3 = [rk_v(t+dt/2.0, x+k2[1]/2.0, v+k2[2]/2.0)*dt,
			  rk_a(t+dt/2.0, x+k2[1]/2.0, v+k2[2]/2.0)*dt]
		k4 = [rk_v(t+dt, x+k3[1], v+k3[2])*dt,
			  rk_a(t+dt, x+k3[1], v+k3[2])*dt]
		dx = (k1[1] + 2.0*k2[1] + 2.0*k3[1] + k4[1])/6.0
		dv = (k1[2] + 2.0*k2[2] + 2.0*k3[2] + k4[2])/6.0
		x = x + dx
		v = v + dv
	end
end
rk()

# eof
