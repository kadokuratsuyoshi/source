#!/usr/bin/env python
# -+- coding:utf-8 -*-
import sys

def main():
    argv = sys.argv
    argc = len(argv)

    try:
        file_data = open(argv[1], 'r')
    except IOError:
        print('cannot open', argv[1])
        sys.exit()

    line = file_data.readline()
    while line:
        #line = line.strip()
        #line = line.strip("a")
        #line = line.lstrip("a")
        #line = line.rstrip("a")
        #line = line.replace("line", "str")
        #print(line)
        sys.stdout.write(line)
        line = file_data.readline()
    file_data.close()

if __name__ == '__main__':
    main()
# eof
