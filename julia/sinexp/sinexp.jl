#=
    program:    sinexp.jl
    conents:    moving wave-packet
    make:       julia sinexp.jl
    usage:      sinexp()
=#
using Plots
anim=Animation()
f(x,t)=sin(10x)*exp(-(x-t)*(x-t))
#x=-2pi:0.1:2pi
x=[i for i in -2pi:0.1:2pi]

for t in -2pi:0.1:2pi
    f_result=map(f,x,[t for i in 1:length(x)])
    plot(f_result,
        xlims=(0,130),
        ylims=(-1.0,1.0),
        size=(500,300))
    frame(anim)
end
gif(anim)
