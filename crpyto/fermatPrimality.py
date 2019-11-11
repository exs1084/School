# -*- coding: utf-8 -*-
"""
Created on Wed Apr 10 18:01:56 2019

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
    
def isPrime(p):
    for x in range(2, floor(sqrt(p))):
        if p % x == 0:
            return False
    return True

def fermat(p, s):
    subset = sample(range(2, p-1), s)
    passedGcd = False
    for a in subset:
        if gcd(a, p) is 1:
            passedGcd = True
            if sqAndMul(a, p-1, p) is not 1:
                return False
    if passedGcd:#catch cases where all s values for a were thrown out, often 
        return True#due to p being even
    else:
        return False

def isCarmichael(C):
    for a in range(C):
        if gcd(a, C)==1:
            if sqAndMul(a, C-1, C) is not 1:
                return False
    return True

def main():
    start_time = time.time()
    start = 10**7
    security = 15
    carmichaels = []
    while(True):
        if(fermat(start, security)):#likely prime
            if(not isPrime(start)):#not prime
                #if(isCarmichael(start)):#is car
                carmichaels.append(start)
                if(len(carmichaels)==5):
                    break
        start-=1
        if(start < security+3):
            break
    print(carmichaels)
    print("--- %s seconds total" % (time.time() - start_time))
    
    
if __name__ == "__main__":
    main()