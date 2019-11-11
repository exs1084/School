# -*- coding: utf-8 -*-
"""
Created on Sat Apr 13 10:58:08 2019

@author: Erikson
"""
from math import sin, cos, asin, sqrt, radians, inf
import numpy as np
from matplotlib import pyplot
import time
import pandas

def distance(loc1, loc2):#in [lat, long]
    dist = 0
    for x in range(len(loc1)):
        dist += (loc1[x]-loc2[x])**2
    return sqrt(dist)

def main():
    #read input
    start_time = time.time()
    minPts = 7
    fileName = "DATA_SET_J__v933.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    line = dataFile.readline()#skip column titles
    while line:#read data
        stripline = line.strip().split(',')
        entry = [float(x) for x in stripline[1:]]
        data.append(entry)
        line = dataFile.readline()
    dists = []
    for x in range(len(data)):#for each point
        closest = [inf for x in range(minPts)]
        for y in range(len(data)):#for each other point
            if x==y:
                continue
            newdist = distance(data[x], data[y])#find distance
            for z in range(len(closest)):#compare to n closest distances
                if newdist < closest[z]:#if closer than one
                    for q in range(len(closest)-1, z, -1):#shift further distanes by 1 index
                        closest[q] = closest[q-1]
                    closest[z] = newdist#add new close distance to list
                    break
        dists.append(closest[-1])
    
    dists.sort()
    
    df1 = pandas.DataFrame({"dist": dists, "point": range(len(dists))})
    pyplot.figure()
    ax1 = df1.plot(x = "point", y = "dist", title = "distance to %sth closest neighbor"%minPts)
    ax1.set_xlabel("point")
    ax1.set_ylabel("distance")
    
    print("done")
    print("--- %s seconds total" % (time.time() - start_time))

if __name__ == "__main__":
    main()