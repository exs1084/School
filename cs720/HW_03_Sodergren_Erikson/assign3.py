# -*- coding: utf-8 -*-
"""
Created on Tue Feb 12 18:06:04 2019

@author: Erikson
"""

import matplotlib.pyplot as pyplot
import pandas

def main():
    fileName = "assign3Data.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    maxSpeed = 0
    minSpeed = 9999
    bestThresh = 0
    bestCost = 9999
    totalPos = 0
    totalNeg = 0
    if(fileName == "assign3Data.csv"):
        line = dataFile.readline()#skip the column title from the file
    while line:#read data
        driver = line.strip().split(',')#form into lists of 2 elements
        driver[0] = float(driver[0])
        driver[0] = round(driver[0]*2)/2#round to nearest half
        maxSpeed = max(maxSpeed, driver[0])
        minSpeed = min(minSpeed, driver[0])
        driver[1] = int(driver[1])
        if(driver[1] == 0):
            totalNeg+=1
        else:
            totalPos+=1
        data.append(driver)
        line = dataFile.readline()
    allCosts = list()
    allThresh = list()
    ROCTP = list()
    ROCFP = list()
    bestFP=0
    bestFN=0
    for x in range(int((maxSpeed - minSpeed)*2)):
        thresh = minSpeed + x/2
        leftData = [y for y in data if y[0] < thresh]
        rightData = [y for y in data if y[0] >= thresh]
        FP = 0
        FN = 0
        for left in leftData:
            if(left[1] == 1):
                FN+=1
        for right in rightData:
            if(right[1] == 0):
                FP+=1
        allCosts.append(FN+FP)
        allThresh.append(thresh)
        ROCFP.append(FP/totalNeg)#false alarm rate
        ROCTP.append((len(rightData)-FP)/totalPos)#true posititve rate
        if(FN+FP <= bestCost):
            bestThresh = thresh
            bestCost = FN+FP
            bestFN=FN
            bestFP=FP
    print("best threshold is %s with a cost function value of %s"%(bestThresh, bestCost))
    print("%s false positives, %s false negatives"%(bestFP, bestFN))
    
    df1 = pandas.DataFrame({"cost": allCosts, "thresholds": allThresh})
    pyplot.figure()
    ax1 = df1.plot(x = "thresholds", y = "cost", title = "mcost function at different thresholds")
    ax1.set_xlabel("thresholds")
    ax1.set_ylabel("cost function")
    

    
    df2 = pandas.DataFrame({"TPR": ROCTP, "FAR": ROCFP})
    pyplot.figure()
    ax2 = df2.plot(x = "FAR", y = "TPR", title = "ROC curve")
    ax2.set_xlabel("False Alarm Rate")
    ax2.set_ylabel("True Positive Rate")
    bestIdxs = [i for i, e in enumerate(allCosts) if e == bestCost]
    for x in bestIdxs:
        ax2.plot(ROCFP[x], ROCTP[x], 'or')
        
if __name__ == "__main__":
    main()