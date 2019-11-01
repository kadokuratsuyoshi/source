#!/usr/bin/env python
#-*- coding:utf-8 -*-
import numpy as np
from scipy.integrate import odeint
np.set_printoptions(threshold=np.inf)
import matplotlib.pyplot as plt

def model(y,t):
    a = 1.0
    b = 10.0
    ddtdydt = -a*y[1] -b*y[0]
    return [y[1], ddtdydt]

def main():
    y0 = [10,0]
    n = np.linspace(0,20,num=1000)
    y = odeint(model,y0,n)
    #print(y)
    plt.plot(y)
    plt.show()

if __name__ == '__main__':
    main()
# eof
