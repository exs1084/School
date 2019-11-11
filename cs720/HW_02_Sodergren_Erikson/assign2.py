# -*- coding: utf-8 -*-
"""
Created on Tue Feb  5 14:45:30 2019

@author: Erikson
"""

import matplotlib.pyplot as pyplot
import pandas
from numpy import var

def main():
    fileName = "Speed_Data.csv"
    #fileName = "Mystery_Data.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    if(fileName == "Mystery_Data.csv"):
        line = dataFile.readline()#skip the column title from the file
    minVal=999
    maxVal=0
    while line:#read data
        temp = float(line.rstrip())
        tempInt = int(round(temp))
        if(fileName == "Speed_Data.csv"):#quantize data
            tempInt = int(tempInt/2)
            tempInt = tempInt*2
        maxVal = max(maxVal, tempInt)
        minVal = min(minVal, tempInt)
        data.append(tempInt)
        line = dataFile.readline()
    
    #begin otsus
    bestThresh = 0
    bestVar = 999
    allVars = list()
    
    for x in range(minVal, maxVal):
        leftData = [y for y in data if y < x]
        rightData = [y for y in data if y >= x]
        weightLeft = len(leftData)/len(data)
        weightRight = 1-weightLeft
        varLeft = var(leftData)
        varRight = var(rightData)
        mixVar = varLeft*weightLeft + varRight*weightRight
        allVars.append(mixVar)
        if(mixVar <= bestVar):
            bestVar = mixVar
            bestThresh = x
    print("the best threshold is at %s mph, mixed variance = %s" %(bestThresh, bestVar))
    
    #histogram of data
    df = pandas.DataFrame({"speeds": data})
    hist = df.hist(bins=range(minVal, maxVal+1))
    
    #graph threshold vs variance
    df2 = pandas.DataFrame({"variance": allVars, "thresholds": range(minVal, maxVal)})
    pyplot.figure()
    ax2 = df2.plot(x = "thresholds", y = "variance", title = "mixed variance at different thresholds")
    ax2.set_xlabel("thresholds")
    ax2.set_ylabel("mixed variance")
    ax2.plot(bestThresh, bestVar, 'or')
        
        

if __name__ == "__main__":
    main()