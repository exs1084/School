# -*- coding: utf-8 -*-
"""
Created on Sat Apr  6 13:57:39 2019

@author: exs1084
"""
from math import sin, cos, asin, sqrt, radians, inf

def distance(loc1, loc2):#in [lat, long]
    dist = 0
    for x in range(len(loc1)):
        dist += (loc1[x]-loc2[x])**2
    return sqrt(dist)


def KNN(datapoint, neighbors):
    closestNeighbor = 0
    closestDist = inf
    for x in range(len(neighbors)):
        newDist = distance(datapoint, neighbors[x])
        if newDist < closestDist:
            closestDist = newDist
            closestNeighbor = x
    return closestNeighbor
    
    
def main():
    fileName = "HW09_CLASSIFY_THESE_2185.csv"
    dataFile = open(fileName, "r")#open file
    neighborFileName = "agglomCenters.csv"
    neighborFile = open(neighborFileName, "r")
    data = list()
    centers = list()
    line = neighborFile.readline()
    line = neighborFile.readline()#skip header row
    while line:#read data
        stripline = line.strip().split(',')
        neighbor = [float(x) for x in stripline[1:]]
        centers.append(neighbor)
        line = neighborFile.readline()
    line = dataFile.readline()
    while line:#read data
        stripline = line.strip().split(',')
        entry = [float(x) for x in stripline]
        data.append(entry)
        line = dataFile.readline()
    for x in data:
        thisClust = KNN(x[1:], centers)
        print("datapoint %s is nearest to centerpoint %s"%(x[0], thisClust))
    
    
    
if __name__ == "__main__":
    main()