# -*- coding: utf-8 -*-
"""
Created on Wed Mar 27 18:40:34 2019

@author: Erikson
"""
from math import sin, cos, asin, sqrt, radians, inf
import numpy as np
import matplotlib.pyplot as plt
import time

def distance(loc1, loc2):#in [lat, long]
    dist = 0
    for x in range(len(loc1)):
        dist += (loc1[x]-loc2[x])**2
    return sqrt(dist)
    
def updateDists(centers, dists, lowIdx, highIdx):#improve by doing it the old way~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~change only necessary distances
    N = len(centers)
    del dists[highIdx]
    for x in range(N):#initialize distances
        if x == lowIdx:
            dists[x][x] = inf
            for y in range(x):
                dists[x][y] = distance(centers[x], centers[y])
        else:
            del dists[x][highIdx]
            dists[x][lowIdx] = distance(centers[x], centers[lowIdx])
            
    return dists
                
def updateCenters(clusters):
    centers = []
    for clust in clusters:
        size = len(clust)
        center = [0 for value in clust[0]]
        for record in clust:
            for x in range(len(record)):
                center[x] += record[x]
        for x in range(len(center)):
            center[x] /= size
        centers.append(center)
    return centers
    
def main():
    #read input
    start_time = time.time()
    fileName = "HW_PCA_SHOPPING_CART_v850_KEY.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    centers = list()
    line = dataFile.readline()
    line = dataFile.readline()#skip column titles
    while line:#read data
        stripline = line.strip().split(',')
        entry = [float(x) for x in stripline[1:]]
        data.append([entry])
        centers.append(entry)
        line = dataFile.readline()
    N = len(data)#input data length
    dists = [[inf for x in range(N)] for x in range(N)]
    for x in range(N):#initialize distances
        for y in range(x):
            if x == y:
                dists[x][y] = inf
            else:
                dists[x][y] = distance(data[x][0], data[y][0])
    smallerMergedIn = []
    while True:#main loop
        lowX = 0
        lowY = 0
        lowDist = inf
        currN = len(centers)
        if currN <= 1:
            break
        for x in range(currN):#find closest pair
            for y in range(x):
                if dists[x][y] < lowDist:#since distance func is symmetric, x is always the lower index
                    lowDist = dists[x][y]
                    lowY = y
                    lowX = x
        data[lowY] += data[lowX]#merge higher idx into lower idx
        smallerMergedIn.append(len(data[lowX]))
        del data[lowX]#delete lower idx
        centers = updateCenters(data)
        updateDists(centers, dists, lowY, lowX)
        if(len(centers)%100==0):
            print("%s clusters left"%len(centers))
            print("--- %s seconds so far" % (time.time() - start_time))
        if(len(centers)==6):
            for x in range(6):
                formattedCenter = ["%.2f"%y for y in centers[x]]
                print("cluster %s is size %s with prototype %s"%(x, len(data[x]), formattedCenter))
    print("done")
    print("--- %s seconds total" % (time.time() - start_time))
    print(smallerMergedIn[-20:])
    
    
    
if __name__ == "__main__":
    main()