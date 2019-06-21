#!/usr/bin/env python
# -+- coding:utf-8 -*-
import sys

def main():
    argv = sys.argv
    argc = len(argv)
    n = 0
    while argc !=0:
        print '%s' %argv[n]
        n += 1
        argc -= 1

if __name__ == '__main__':
    main()
# eof
