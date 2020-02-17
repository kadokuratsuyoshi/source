import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.integrate import odeint

def system(theta, t, b, g, l, m):
    theta1 = theta[0]
    theta2 = theta[1]
    dtheta1_dt = theta2
    dtheta2_dt = -(b/m)*theta2 - g*math.sin(theta1)
    dtheta_dt = [dtheta1_dt, dtheta2_dt]
    return dtheta_dt

b = 0.05
g = 9.81
l = 1
m = 1
theta_0 = [0, 3]
t = np.linspace(0, 20, 250)
theta = odeint(system, theta_0, t, args = (b, g, l, m))

'''
f = 1
for i in range(0, 250):
    filename = str(1000+f)+'.png'
    f = f + 1
    plt.figure()
    plt.plot([10, l*math.sin(theta[i, 0]) + 10], [10, 10-l*math.cos(theta[i, 0]) ], marker = "o")
    plt.xlim([0, 20])
    plt.ylim([0, 20])
    plt.savefig(filename)
'''

plt.plot(t, theta[:, 0], 'b-')
plt.plot(t, theta[:, 1], 'r--')
plt.show()

