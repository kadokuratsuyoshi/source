#!/usr/bin/python3
#-*- coding:utf-8 -*-
# to solve x^2=2, x=sqrt(2), -sqrt(2)
# optimizing by the variational method, delta f(x) = 0
# https://blog2.cct-inc.co.jp/blog/machine-learning/pytorch/

import torch
import torch.nn as nn
import torch.optim as optim

x = torch.tensor([1.], requires_grad=True)
loss_func = nn.MSELoss()
optimizer = optim.SGD([x], lr=0.01)
target = torch.tensor([0.])

for iter_num in range(100):
    optimizer.zero_grad()
    y = x ** 2 - 2
    #y = x ** 2 - 3 
    #y = x ** 2 - 4 
    #y = x ** 2 - 5
    loss = loss_func(y, target)
    loss.backward()
    print("{0}: x={1:.7f}, y={2:.7f}, dL/dx={3:.7f}".format(
		iter_num,
		x.data.numpy()[0],
		y.data.numpy()[0],
		x.grad.data.numpy()[0])
	)
    optimizer.step()

print("x={0:.7f}".format(x.data.numpy()[0]))
