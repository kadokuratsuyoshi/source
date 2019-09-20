#!/usr/bin/bash env
# -*- coding:utf-8 -*-
#
#   program:    lorentz3.py
#   contents:   plot the lorentz-atractor
#   make:
#   usage:      python lorentz3.py
import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.axes3d import Axes3D
 
def lorenz(u, t, p, b, r):
    x = u[0]
    y = u[1]
    z = u[2]
    dxdt = -p * x + p * y
    dydt = -x * z + r * x - y
    dzdt = x * y - b * z
    return([dxdt, dydt, dzdt])

def main():
    p =10
    b = 8.0 / 3.0
    r = 28
    x0 = [0.1, 0.1, 0.1]
    dt = 0.01
    T = 40.0
    times = np.arange(0.0, T, dt)
    args = (p, b, r)
    orbit = odeint(lorenz, x0, times, args)

    xorbit = []
    yorbit = []
    zorbit = []
    for x, y, z in orbit:
        xorbit.append(x)
        yorbit.append(y)
        zorbit.append(z)
    fig = plt.figure()
    ax = fig.gca()
    ax.plot(times, xorbit)
    plt.show()

if __name__ == '__main__':
    main()
# eof
