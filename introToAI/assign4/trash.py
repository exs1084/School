# -*- coding: utf-8 -*-
"""
Created on Sun Apr 14 14:49:06 2019

@author: Erikson
"""
from random import sample
from math import sqrt, gcd, floor
import time

def sqAndMul(base, exp, mod):
    result = 1
    binlist = [int(x) for x in '{:b}'.format(exp)]
    for x in binlist:
        result**=2
        result %=mod
        if x is 1:
            result *= base
            result %=mod
    return result
    
def main():
    for x in range(65, 65+26):
        print("%s: %s"%(chr(x), sqAndMul(x, 11, 3763)))
    
if __name__ == '__main__':
	main()