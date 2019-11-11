# -*- coding: utf-8 -*-
"""
Created on Tue Feb  5 14:45:30 2019

@author: Erikson
"""

import matplotlib.pyplot as pyplot
import pandas
from numpy import var

def varianceCalc(input, firstBin, binSize):
    data = list()
    binVal = firstBin
    for bin in input:
        for point in range(bin):
            data.append(binVal)
        binVal += binSize
    return var(data)
    

def main():
    #fileName = "Speed_Data.csv"
    fileName = "Mystery_Data.csv"
    dataFile = open(fileName, "r")
    data = list()
    histData = list()
    totalPoints = 0
    for x in range(21):
        data.append(0)
    line = dataFile.readline()
    if(fileName == "Mystery_Data.csv"):
        line = dataFile.readline()#skip the column title from the file
    #bin the data. bin index is (value-38)/2. all contained values will be
    #treated as (index*2)+38
    
    
    
    #might have misunderstood...perhaps just meant to convert each number to its closes
    #bin minimum, and keep a single list?
    while line:
        temp = float(line.rstrip())
        tempInt = int(round(temp))
        histData.append(tempInt)
        tempIdx = int((tempInt-38)/2)
        data[tempIdx]+=1
        totalPoints += 1
        line = dataFile.readline()
    
    #begin otsus
    bestThresh = 0
    bestVar = 999
    allVars = list()
    
    for x in range(40, 80, 2):
        idxLeft = int((x-38)/2)
        leftData = data[:idxLeft]
        rightData = data[idxLeft:]
        weightLeft = sum(leftData)/totalPoints#counting numBins, not elements
        weightRight = 1-weightLeft
        varLeft = varianceCalc(leftData, x, 2)#very very wrong...maybe just round into bins but keep as a list?
        varRight = varianceCalc(rightData, x+2, 2)
        mixVar = varLeft*weightLeft + varRight*weightRight
        allVars.append(mixVar)
        if(mixVar <= bestVar):
            bestVar = mixVar
            bestThresh = x
    print("the best threshold is at %s mph, mixed variance = %s" %(bestThresh, bestVar))
    
    #histogram of data
    df = pandas.DataFrame({"speeds": histData})
    hist = df.hist(bins=range(38, 82, 2))
    
    #graph threshold vs variance
    df2 = pandas.DataFrame({"variance": allVars, "thresholds": range(40, 80, 2)})
    pyplot.figure()
    ax2 = df2.plot(x = "thresholds", y = "variance", title = "mixed variance at different thresholds")
    ax2.set_xlabel("thresholds")
    ax2.set_ylabel("mixed variance")
        
        

if __name__ == "__main__":
    main()