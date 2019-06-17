#!/usr/bin/env python
# -+- coding:utf-8 -*-
import sys
import math

def main():
    fname = 'wf.txt'

    print("output")
    with open(fname, 'w') as file_data:
        t = -100
        x = 0
        v = 0
        while t < 100:
            x = x + t
            #v = math.sin(math.radians(t))
            v = math.sin(((2.0 * math.pi)/20)*t)*math.exp(-t*t*1.0e-3)
            #result = '{:.7f} {:.7f} {:.7f}\n'.format(t, x, v)
            result = '{0} {1} {2}\n'.format(t, x, v)
            file_data.write(result)
            t += 1
        file_data.close()

    print("completed!")

if __name__ == '__main__':
    main()
# eof
