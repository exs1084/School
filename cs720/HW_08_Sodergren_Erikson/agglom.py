# -*- coding: utf-8 -*-
"""
Created on Wed Mar 27 18:40:34 2019

@author: Erikson
"""
from math import sin, cos, asin, sqrt, radians, inf
from mpl_toolkits.basemap import Basemap
import numpy as np
import matplotlib.pyplot as plt

EARTH_RADIUS = 6371#average radius in km

def hav(theta):#haversine subFunc
    return pow(sin(theta/2),2)
    

def distance(loc1, loc2):#in [lat, long]
    lat1 = radians(loc1[0])
    lat2 = radians(loc2[0])
    lon1 = radians(loc1[1])
    lon2 = radians(loc2[1])
    temp =  hav(abs(lat2-lat1))+cos(lat1)*cos(lat2)*hav(abs(lon2-lon1))
    return EARTH_RADIUS * 2 * asin(min(1,sqrt(temp)))
    
def updateDists(dists, mergeInIdx, removeIdx):
    size = len(dists)
    for x in range(size):#for each cluster
        if x is not removeIdx:
            if x is mergeInIdx:
                for y in range(size):
                    if y is mergeInIdx:
                        dists[x][y] = inf
                    else:#update distance from newly merged to other clusters
                        dists[x][y] = min(dists[x][y], dists[removeIdx][y])
                del dists[x][removeIdx]
            else:#update distance to newly merged cluster
                dists[x][mergeInIdx] = min(dists[x][mergeInIdx], dists[x][removeIdx])
                del dists[x][removeIdx]
    #remove dists[removeIdx]
    del dists[removeIdx]
    
def main():
    #read input
    fileName = "lat_long_pts.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    line = dataFile.readline()#skip column titles
    while line:#read data
        stripline = line.strip().split(',')
        entry = [float(x) for x in stripline]
        data.append([entry])
        line = dataFile.readline()
    N = len(data)#input data length
    dists = [[inf for x in range(N)] for x in range(N)]
    for x in range(N):#initialize distances
        for y in range(N):
            if x == y:
                dists[x][y] = inf
            else:
                dists[x][y] = distance(data[x][0], data[y][0])
    while True:#main loop
        lowX = 0
        lowY = 0
        lowDist = inf
        currN = len(data)
        if currN <= 12:
            break
        for x in range(currN):#find closest pair
            for y in range(currN):
                if dists[x][y] < lowDist:#since distance func is symmetric, x is always the lower index
                    lowDist = dists[x][y]
                    lowY = y
                    lowX = x
        data[lowX] += data[lowY]#merge higher idx into lower idx
        del data[lowY]#delete lower idx
        updateDists(dists, lowX, lowY)
    for x in range(12):
        print("size of cluster %s is %s"%(x, len(data[x])))
    m = Basemap(projection='eck4',lon_0=-350,resolution='c')
    m.drawcoastlines(color='#442299',zorder=1)
    m.drawmapboundary(zorder=0)
    colors = ['r', 'b', 'g', 'c', 'm', 'y', 'k', 'w', "pink", "grey", "brown", "orange"]
    for i in range(12):#12 clusters, 12 colors
        lats = [capital[0] for capital in data[i]]
        lons = [capital[1]for capital in data[i]]
        x, y = m(lons,lats)
        m.scatter(x,y,3,marker='o',color=colors[i],zorder=2)
    plt.savefig('map.png', dpi=400)
    
    
if __name__ == "__main__":
    main()