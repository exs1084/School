# -*- coding: utf-8 -*-
"""
Created on Mon Mar 18 12:35:50 2019

@author: Erikson
"""

n = 30

for x in range(2,n):
    temp = x
    order = 1
    while(True):
        temp = (temp * x) % n
        order += 1
        if(temp == 1):
            print("order of %s mod %s is %s"%(x, n, order))
            break
        if(temp == x):
            print("order of %s mod %s is nonexistant"%(x, n))
            break