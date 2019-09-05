#=
  program:  ode.jl
  contents: test for dumping oscillator
  make:     julia ode.jl
  usage:    ode()
=#
using Plots;gr()
pyplot()
using DifferentialEquations
function ode(du,u,p,t)
  x, v = u
  du[1] = v
  du[2] = -x - 0.5v
end
u0 = [0.0, 1.0]
#u0 = [1.5]
tspan = (0.0, 20.0)
p = 0.5
prob = ODEProblem(ode,u0,tspan, p)
sol = solve(prob)
using Plots
#plot(sol)
plot(sol,label=["x(t)" "v(t)"])
