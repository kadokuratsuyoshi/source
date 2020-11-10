#!/usr/bin/env python3
#-*- coding:utf-8 -*-
def rk_v(t, x, v):
    return v

def rk_a(t, x, v):
    k=1.0
    m=1.0
    gamma=1.0
    return ((-k*x-gamma*v)/m)

def main():
    t=0.0
    x=1.0
    v=0.0
    dt=0.01
    rdx=[0,1,2,3]
    rdv=[0,1,2,3]
    n=1000
    for i in range(n):
        print("%f %f %f" % (t, x, v))
        rdx[0] = dt * rk_v(t, x, v )
        rdx[1] = dt * rk_v(t+dt/2, x+rdx[0]/2, v+rdv[0]/2)
        rdx[2] = dt * rk_v(t+dt/2, x+rdx[1]/2, v+rdv[1]/2)
        rdx[3] = dt * rk_v(t+dt,   x+rdx[2],   v+rdv[2])
        rdv[0] = dt * rk_a(t, x, v)
        rdv[1] = dt * rk_a(t+dt/2, x+rdx[0]/2, v+rdv[0]/2)
        rdv[2] = dt * rk_a(t+dt/2, x+rdx[1]/2, v+rdv[1]/2)
        rdv[3] = dt * rk_a(t+dt,   x+rdx[2],   v+rdv[2])
        dx = (rdx[0]+rdx[1]*2+rdx[2]*2+rdx[3])/6
        dv = (rdv[0]+rdv[1]*2+rdv[2]*2+rdv[3])/6
        t = t + dt
        x = x + dx
        v = v + dv

if __name__ == '__main__':
    main()
