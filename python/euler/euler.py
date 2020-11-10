#!/usr/bin/env python3
# -*- coding:utf-8 -*-
def main():
    t = 0
    x = 1.0
    v = 0.0
    dt = 0.01
    k = 1.0
    m = 1.0
    gamma = 1.0
    n = 1000
    for i in range(n):
        print("%f %f %f" % (t, x, v))
        dx = dt * v
        dv = dt * ((-k*-gamma*v)/m)
        t = t + dt
        x = x + dx
        v = v + dv
        
if __name__ == '__main__':
    main()
