# -*- coding: utf-8 -*-
"""
Created on Mon Apr 22 12:12:00 2019

@author: Erikson
"""

#how to use a variable for an import?
#pickle package?
#python does "python filename inputs" not "filename inputs"...might need script to run python?
import importlib
import sys

def main(hypothesis, data):
    mod = importlib.import_module(hypothesis)
    mod.main(data)
    
    
if __name__ == '__main__':
    args = sys.argv
    #args = ["predict.py", "Classifier", "test.dat"]
    main(args[1], args[2])