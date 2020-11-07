BEGIN {
    t = 0.0
    x = 1.0
    v = 0.0
    dt = 0.01
    dx = 0.0
    dv = 0.0
    k = 1.0
    m = 1.0
    gamma = 1.0
    for (i=0; i<1000; i++) {
        printf "%f, %f, %f\n", t, x, v
        dx = dt * v
        dv = dt * ((-k*x-gamma*v)/m)
        t = t + dt
        x = x + dx
        v = v + dv
    }
}
