#! ruby -Ku
N = 10
t = 0
dt = 0.01
x = 1.0
v = 0.0
dx = 0.0
dv = 0.0
k = 1.0
m = 1.0
a = 1.0
while t <= N do
    dx = v * dt
    dv = (-1.0 * (k/m) * x - (a/m) * v) * dt
    x = x + dx 
    v = v + dv
    t = t + dt
    print t, " ", x, " ", v, "\n"
end
