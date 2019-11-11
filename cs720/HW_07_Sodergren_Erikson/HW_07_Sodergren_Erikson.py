c
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 19 18:08:37 2019

@author: Erikson
"""
from math import sqrt
import math
import random
from datetime import datetime
import matplotlib.pyplot as pyplot
import pandas

def distance(loc1, loc2):
    temp = pow(loc1[0]-loc2[0], 2)
    temp +=pow(loc1[1]-loc2[1], 2)
    temp +=pow(loc1[2]-loc2[2], 2)
    temp +=pow(loc1[3]-loc2[3], 2)
    return sqrt(temp)

def SSE(centers, clusters, k):
    sum = 0
    for x in range(k):
        for loc in clusters[x]:
            sum += pow(distance(loc, centers[x]), 2)
    return sum

def findCenter(cluster):
    center = [0,0,0,0]
    clustSize = len(cluster)
    for loc in cluster:
        center[0]+=loc[0]
        center[1]+=loc[1]
        center[2]+=loc[2]
        center[3]+=loc[3]
    center[0] /=clustSize
    center[1] /=clustSize
    center[2] /=clustSize
    center[3] /=clustSize
    return center

def newCenters(clusters, oldCenters):
    newCenters = []
    biggest = -1
    sizeBiggest = -1
    empty = -1
    for x in range(len(clusters)):
        if len(clusters[x])== 0:
            empty = x
        if len(clusters[x])>sizeBiggest:
            biggest = x
            sizeBiggest = len(clusters[x])
    if empty != -1:
        furthest = 0
        distFurthest = 0
        for x in range(len(clusters[biggest])):
            if distance(oldCenters[biggest], clusters[biggest][x]) > distFurthest:
                furthest = x
                distFurthest = distance(oldCenters[biggest], clusters[biggest][x])
        clusters[empty].append(clusters[biggest][furthest])
        del clusters[biggest][furthest]
    
    for x in clusters:
        newCenters.append(findCenter(x))
    return newCenters

def newClusters(data, centers, k):
    clusters = [[] for x in range(k)]
    for x in data:
        closest = -1
        bestDist = math.inf
        for y in range(k):
            dist = distance(x, centers[y])
            if dist < bestDist:
                bestDist = dist
                closest = y
        clusters[closest].append(x)
    return clusters

def sortFunc(entry):
    return len(entry)

def kmeans(k, data):
    size = len(data)
    bestClusters = []
    bestCenters = []
    bestError = math.inf
    start=datetime.now()
    for x in range(1000):
        centers = random.sample(data, k)
        clusters = newClusters(data, centers, k)
        error = SSE(centers, clusters, k)
        while(True):
            #find new center
            centers = newCenters(clusters, centers)
            #find new clusters
            clusters = newClusters(data, centers, k)
            #find sse
            newError = SSE(centers, clusters, k)
            #check if sse changed by small enough proportion, break if so
            if error / newError < 1.1:
                error = newError
                break
            error = newError
        if error < bestError:
            bestError = error
            bestClusters = clusters
            bestCenters = centers
    sizes = [len(x) for x in bestClusters]
    time = datetime.now()-start
    #now print a table
    tempClusts = bestClusters
    for x in range(k):
        tempClusts[x].append(x)
    tempClusts.sort(key=sortFunc)
    tempCounter = 1
    print("ID\tA1\tA2\tA3\tA4\tSize\tSSE")
    for x in tempClusts:
        y = bestCenters[x[-1]]
        tempError = SSE([y], [x[0:-1]], 1)
        print("%s\t%.1f\t%.1f\t%.1f\t%.1f\t%s\t%s\t"%(tempCounter, y[0], y[1], y[2], y[3], len(x)-1, tempError))
        tempCounter+=1
    print("\n")
    return [bestError, sizes, bestCenters, time]

def main():
    fileName = "HW_K_MEANS__DATA_v2185.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    while line:#read data
        stripline = line.strip().split(',')
        entry = [float(x) for x in stripline[1:]]
        entry.append(int(stripline[0]))
        data.append(entry)
        line = dataFile.readline()

    errors = []
    times = []
    for x in range(15):
        results = kmeans(x+1, data)
        errors.append(results[0])
        times.append(results[3].total_seconds())
    #do the graphing
    df1 = pandas.DataFrame({"SSE": errors, "k": [x+1 for x in range(15)]})
    pyplot.figure()
    ax1 = df1.plot(x = "k", y = "SSE", title = "total SSE for each value of k")
    ax1.set_xlabel("K")
    ax1.set_ylabel("SSE")
    
    df2 = pandas.DataFrame({"run time": times, "k": [x+1 for x in range(15)]})
    pyplot.figure()
    ax2 = df2.plot(x = "k", y = "run time", title = "Runtime for each value of k")
    ax2.set_xlabel("K")
    ax2.set_ylabel("Runtime(seconds)")
        


if __name__ == "__main__":
    main()