# https://nbviewer.jupyter.org/github/Lirimy/simulation/blob/master/diffusion.ipynb
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.animation import FuncAnimation
#%matplotlib inline
#from IPython.display import HTML
import torch
import torch.nn as nn
from torch.autograd import Variable

#dtype = torch.FloatTensor # CPU
dtype = torch.cuda.FloatTensor # GPU

class Net(nn.Module):
    def __init__(self, sigma=3.):
        super(Net, self).__init__()
        self.setSigma(sigma)
        self.setGrid(np.random.randn(60,80))

    def setSigma(self, sigma=3.):
        """
        set sigma and automatically generate transpose tensor
        sigma: float-like (default: 3.)
        """
        self.sigma=sigma
        m = int(min(64-1, max(1, 2*sigma)))
        self.m = m
        ruler = np.linspace(-m, m, num=2*m+1, endpoint=True)
        X1, X2 = np.meshgrid(ruler, ruler)
        dist = np.exp(-(X1*X1+X2*X2)/(2*sigma*sigma))
        dist = dist / dist.sum()
        self.conv = nn.Conv2d(1, 1, 2*m+1, padding=m, bias=False)
        self.conv.weight = nn.Parameter(torch.Tensor([[dist]]).type(dtype),
                                        requires_grad=False)

    def getSigma(self):
        """
        get sigma
        return: float-like
        """
        return self.sigma

    def getWeight(self):
        """
        get transpose matrix
        return: Numpy 2-D Array
        """
        return self.conv.weight.data.cpu().numpy()[0, 0]

    def setGrid(self, x):
        """
        set grid value
        x: Numpy 2-D Array
        """
        self.gridMin = min(0, x.min())
        self.gridMax = max(0, x.max())
        self.grid = Variable(torch.Tensor([[x]]).type(dtype),
                             requires_grad=False)

    def getGrid(self):
        """
        get grid value
        return: Numpy 2-D Array
        """
        return self.grid.data.cpu().numpy()[0, 0]

    def step(self):
        """
        update grid by convolution of weight
        """
        self.grid = self.conv(self.grid)

    def showGrid(self, autoscale=False, colorbar=True, nocbtick=False,
                 notick=True):
        """
        show grid
        return: matplotlib object
        """
        fig, ax = plt.subplots()
        self.fig = fig
        self.ax = ax

        if autoscale:
            im = ax.pcolormesh(self.getGrid())
        else:
            im = ax.pcolormesh(self.getGrid(),
                           vmin=self.gridMin, vmax=self.gridMax)

        ax.axis("image")
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="3%", pad="3%")
        fig.tight_layout()

        if colorbar:
            if nocbtick:
                fig.colorbar(im, cax=cax).ax.tick_params(\
                                labelright='off', right='off')
            else:
                fig.colorbar(im, cax=cax)

        if notick:
            ax.tick_params(labelbottom='off', labelleft='off')
            ax.tick_params(bottom='off', left='off')

        self.im = im
        return im

    def updateAnim(self, frame):
        """
        for FuncAnimation
        call showGrid before updateAnim
        frame: number of frame
        return: im
        """
        self.im.set_array(self.getGrid().ravel())
        self.step() # This is here (after set_array), to show the first state
        return self.im

# Initialize Grid
net = Net(3.)
gridsize = (200, 300)
ini = np.zeros(gridsize)
ini[:, :gridsize[1]//2] = 1.
net.setGrid(ini)
net.showGrid()

# Weight
net.setSigma(3.)
plt.plot(net.getWeight()[net.m])
plt.legend(["$\sigma=$" + str(net.getSigma())])
plt.show()

#%%time
# Diffusion
numLoop = 100
for _ in range(numLoop):
    net.step()
net.showGrid(autoscale=True) # Comment-out when benchmark

# Animation
net = Net(10)
gridsize = (200, 300)
ini = np.zeros(gridsize)
ini[:, :gridsize[1]//2] = 1.
net.setGrid(ini)

im  = net.showGrid(nocbtick=True)
anim = FuncAnimation(net.fig, net.updateAnim,
                     blit=False, interval=100, frames=3) #frames=30

#HTML(anim.to_jshtml())
anim.save("anim.gif", writer="imagemagick", fps=10)
#anim.save("anim.mp4", writer="ffmpeg", fps=10)

