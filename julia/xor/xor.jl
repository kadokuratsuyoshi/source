using Flux
using Flux: @epochs, mse, train!

m = Chain(Dense(2,4, tanh),
          Dense(4,1,σ))

loss(x,y) = mse(m(x), y)
ps = params(m)

xs = [[0,0], [0,1], [1,0], [1,1]]
ys = [0,1,1,0]
data = zip(xs, ys)

η = 0.1
opt = Descent(η)

@epochs 10000 train!(loss, ps, data, opt)

for i = 1:4
    println(m(xs[i]))
end
