'''
    program:    ode3.py
    contents:   to solve the Van Der Pal equation
    make:       
    usage:      python ode3.py
    
    x'' + u(x^2-1)x' + x = 0, u = 1
    x'' = -(x^2-1)x' - x
    |
    v
    y0 = x
    y1 = x'
    d/dt y0 = y1
    d/dt y1 = x'' = -(y0^2-1)y1 - y0

'''
import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

t = np.arange(0, 50, 0.1)
data_0 = [1, 1]     # initialize [x, x'']_t =  0

def diff(data, dummy):
    return[data[1], -(data[0]**2-1)*data[1] - data[0]] # return [d/dt y0, d/dt y1]_t

result = odeint(diff, data_0, t)
print(result)
#plt.plot(xdata=data[1],ydata=data[0])
#plt.plot(result)
#plt.show()
# eof
