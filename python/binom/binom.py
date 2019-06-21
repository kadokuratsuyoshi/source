#!/usr/bin/env python
# -+- coding:utf-8 -*-
import numpy as np

def main():
    n, p = 36, .5
    s = np.random.binomial(n, p, 100)
    ave = sum(s)/100
    p24 = sum(s >= 18)/100.
    print(s)
    print(ave)
    print(p24)
 
if __name__ == '__main__':
    main()
# eof
