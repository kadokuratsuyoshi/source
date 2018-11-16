t = 0
n = 1000
dt = 0.01
x = 1.0
v = 0.0
dx = 0.0
dy = 0.0
k = 1.0
m = 1.0
a = 1.0
for t in range(n):
    dx = v * dt
    dv = (-1.0 * (k/m) * x - (a/m) * v) * dt
    x = x + dx
    v = v + dv
    nt = t * dt
    print("%d %f %f %f" % (t, nt, x, v))