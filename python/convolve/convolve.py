import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 10, 500)
y1 = np.sin(x)
y2 = y1 + np.random.randn(500)*0.3

v = np.ones(5)/5.0
y3 = np.convolve(y2, v, mode='same') # default == full, valid, same

plt.plot(x, y1, 'r', linewidth=3)
plt.plot(x, y2, 'b', linewidth=1)
plt.plot(x, y3, 'y', linewidth=1)
plt.show()
