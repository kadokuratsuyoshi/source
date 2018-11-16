BEGIN {
    t = 0.0
    dt = 0.01
    x = 1.0
    v = 0.0
    dx = 0.0
    dv = 0.0
    k = 1.0
    m = 1.0
    a = 1.0
    for (t=dt; t<=10.0; t=t+dt ) {
        dx = v * dt
        dv = (-1.0*(k/m)*x - v*(a/m)) * dt
        x = x + dx
        v = v + dv
        printf "%f,%f,%f\n", t, x, v
    }
}
