function rk_v(t, x, v) {
    return v
}

function rk_a(t, x, v) {
    k = 1.0
    m = 1.0
    gamma = 1.0
    return ( (-k*x-gamma*v)/m )
}

BEGIN {
    t = 0.0
    x = 1.0
    v = 0.0
    dt = 0.01
    dx = 0.0
    dv = 0.0
    for (i=0; i<1000; i++) {
        printf "%f, %f, %f\n", t, x, v
        rdx[1] = dt * rk_v(t, x, v)
        rdv[1] = dt * rk_a(t, x, v)
        rdx[2] = dt * rk_v(t+dt/2.0, x+rdx[1]/2.0, v+rdv1[1]/2.0)
        rdv[2] = dt * rk_a(t+dt/2.0, x+rdx[1]/2.0, v+rdv1[1]/2.0)
        rdx[3] = dt * rk_v(t+dt/2.0, x+rdx[2]/2.0, v+rdv2[2]/2.0)
        rdv[3] = dt * rk_a(t+dt/2.0, x+rdx[2]/2.0, v+rdv2[2]/2.0)
        rdx[4] = dt * rk_v(t+dt, x+rdx[3], v+rdv3[3])
        rdv[4] = dt * rk_a(t+dt, x+rdx[3], v+rdv3[3])
        dx = (rdx[1]+2.0*rdx[2]+2.0*rdx[3]+rdx[4])/6.0
        dv = (rdv[1]+2.0*rdv[2]+2.0*rdv[3]+rdv[4])/6.0
        t = t + dt
        x = x + dx
        v = v + dv
    }
}
