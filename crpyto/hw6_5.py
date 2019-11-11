# -*- coding: utf-8 -*-
"""
Created on Mon Apr  1 17:16:23 2019

@author: Erikson
"""

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
    base = 1234567
    exp = 2345678
    mod = 3333337
    print("result of %s^%s mod %s is: %s"%(base, exp, mod, sqAndMul(base, exp, mod)))
    
    
    
if __name__ == "__main__":
    main()