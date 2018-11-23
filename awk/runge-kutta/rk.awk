function rk_v(t, x, v) {
    return v
}

function rk_a(t, x, v) {
    k = 1.0
    m = 1.0
    a = 1.0
    return ( -1.0*(k/m)*x - (a/m)*v )
}

BEGIN {
    t = 0.0
    dt = 0.01
    x = 1.0
    v = 0.0
    dx = 0.0
    dv = 0.0
    for (t=dt; t<=10.0; t=t+dt ) {
        printf "%f, %f, %f\n", t, x, v
        k1[1] = dt * rk_v(t, x, v)
        k1[2] = dt * rk_a(t, x, v)
        k2[1] = dt * rk_v(t+dt/2.0, x+k1[1]/2.0, v+k1[2]/2.0)
        k2[2] = dt * rk_a(t+dt/2.0, x+k1[1]/2.0, v+k1[2]/2.0)
        k3[1] = dt * rk_v(t+dt/2.0, x+k2[1]/2.0, v+k2[2]/2.0)
        k3[2] = dt * rk_a(t+dt/2.0, x+k2[1]/2.0, v+k2[2]/2.0)
        k4[1] = dt * rk_v(t+dt, x+k3[1], v+k3[2])
        k4[2] = dt * rk_a(t+dt, x+k3[1], v+k3[2])
        dx = (k1[1]+2.0*k2[1]+2.0*k3[1]+k4[1])/6.0
        dv = (k1[2]+2.0*k2[2]+2.0*k3[2]+k4[2])/6.0
        x = x + dx
        v = v + dv
    }
}
