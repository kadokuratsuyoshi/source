t = 0
n = 1000
dt = 0.01
x = 1.0
v = 0.0
dx = 0.0
dv = 0.0
k = 1.0
m = 1.0
gamma = 1.0
for t in range(n):
    print("%f %f %f" % (t, x, v))
    dx = v * dt
    dv = ( (-k*x)/m - (gamma*v)/m ) * dt
    x = x + dx
    v = v + dv
    t = t * dt
