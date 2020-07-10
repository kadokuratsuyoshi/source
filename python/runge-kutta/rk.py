def rk_v( t, x, v ):
    return v

def rk_a( t, x, v):
    k = 1.0
    m = 1.0
    a = 1.0
    return ( -1.0*(k/m)*x - (a/m)*v )

t = 0
tmax = 10.0
dt = 0.01
x = 1.0
v = 0.0
n = 1000
dx = 0.0
dy = 0.0
k1 = [0, 1]
k2 = [0, 1]
k3 = [0, 1]
k4 = [0, 1]
for t in range(n):
    print("%f %f %f" % (t, x, v))
    k1[0] = rk_v( t, x, v ) * dt
    k1[1] = rk_a( t, x, v ) * dt
    k2[0] = rk_v( t+dt/2.0, x+k1[0]/2.0, v+k1[1]/2.0) * dt
    k2[1] = rk_a( t+dt/2.0, x+k1[0]/2.0, v+k1[1]/2.0) * dt
    k3[0] = rk_v( t+dt/2.0, x+k2[0]/2.0, v+k2[1]/2.0) * dt
    k3[1] = rk_a( t+dt/2.0, x+k2[0]/2.0, v+k2[1]/2.0) * dt
    k4[0] = rk_v( t+dt, x+k3[0], v+k3[1] ) * dt
    k4[1] = rk_a( t+dt, x+k3[0], v+k3[1] ) * dt
    dx = (k1[0] + 2.0*k2[0] + 2.0*k3[0] + k4[0]) / 6.0
    dv = (k1[1] + 2.0*k2[1] + 2.0*k3[1] + k4[1]) / 6.0
    x = x + dx
    v = v + dv
    t = t + dt
