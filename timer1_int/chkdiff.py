#! /usr/bin/env python3
#coding: utf-8
#
import sys
curval = 0
preval = -1

lines = sys.stdin.readlines()

for line in lines:
    curval = int(line)
    if (preval==-1):
        pass
    else:
        df = curval - preval
        print("{0}  {1}".format(curval,df))
    preval = curval
