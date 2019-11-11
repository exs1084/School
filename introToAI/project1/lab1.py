# -*- coding: utf-8 -*-
"""
Created on Sun Feb 24 12:15:58 2019

@author: Erikson
"""

import sys
from location import location
import math
from PIL import Image
from copy import deepcopy

pixelWidth = 10.29 #meters
pixelHeight = 7.55

#finds neighbors of a location
def findNeighbors(source, locs):
    ret = []
    x = source.xPix
    y = source.yPix
    coords = []
    coords.append([x, y+1])
    coords.append([x, y-1])
    
    coords.append([x-1, y+1])
    coords.append([x-1, y])
    coords.append([x-1, y-1])
    
    coords.append([x+1, y+1])
    coords.append([x+1, y])
    coords.append([x+1, y-1])
    
    for z in coords:
        if not(z[0]<0 or z[0]>=len(locs) or z[1]<0 or z[1]>=len(locs[0])):
            ret.append(locs[z[0]][z[1]])
    return ret

#finds neighbors using coord rather than loc
def findSimpNeighbors(x, y, locs):
    ret = []
    coords = []
    coords.append([x, y+1])
    coords.append([x, y-1])
    
    coords.append([x-1, y+1])
    coords.append([x-1, y])
    coords.append([x-1, y-1])
    
    coords.append([x+1, y+1])
    coords.append([x+1, y])
    coords.append([x+1, y-1])
    
    for z in coords:
        if not(z[0]<0 or z[0]>=len(locs) or z[1]<0 or z[1]>=len(locs[0])):
            ret.append(z)
    return ret

#adds leafy terrain to footpaths due to fall
def leafify(mapArray, locs):
    for x in range(395):
        for y in range(500):#for each pixel
            if mapArray[x, y][0:3] == (0,0,0):#if footpath
                neighbors = findSimpNeighbors(x, y, locs)
                for z in neighbors:
                    if mapArray[z[0], z[1]][0:3] == (255,255,255):#if neighboring easy forest
                        mapArray[x, y] = (100,100,100, 255)#LEAF
                        locs[x][y].rate = colorToRate(mapArray[x, y])

#adds water to map due to spring floods
def flood(mapArray, locs):
    queue = []
    waterLevel = 0
    for x in range(395):
        for y in range(500):#for each pixel
            if mapArray[x, y][0:3] == (0,0,255):#if water
                waterLevel = locs[x][y].z#record elevation
                for z in findSimpNeighbors(x, y, locs):
                    color = mapArray[z[0], z[1]][0:3]
                    isLand = color != (0,0,255) and color != (205,0,101)
                    alreadyIn = locs[z[0]][z[1]].queuedForSeason
                    floodLevel = locs[z[0]][z[1]].queuedWaterLevel
                    if ((not alreadyIn) or floodLevel < waterLevel) and isLand:
                        queue.append([z[0],z[1],1])#enqueue adjacent land tiles
                        locs[z[0]][z[1]].queuedForSeason = True
                        locs[z[0]][z[1]].queuedWaterLevel = waterLevel
                        
                        
                        
    while(queue):#for each enqueue'd land tile
        x = queue.pop(0)
        waterLevel = locs[x[0]][x[1]].queuedWaterLevel
        if locs[x[0]][x[1]].z <= waterLevel+1:#elevation check
            depth = x[2]
            mapArray[x[0], x[1]] = (100, 50, 0, 255)#FLOOD
            locs[x[0]][x[1]].rate = colorToRate(mapArray[x[0], x[1]])
            if depth<15:
                for z in findSimpNeighbors(x[0], x[1], locs):#enqueue more land
                    color = mapArray[z[0], z[1]][0:3]
                    isLand = color != (0,0,255) and color != (205,0,101)
                    alreadyIn = locs[z[0]][z[1]].queuedForSeason
                    floodLevel = locs[z[0]][z[1]].queuedWaterLevel
                    if ((not alreadyIn) or floodLevel < waterLevel) and isLand:
                        queue.append([z[0],z[1],depth+1, waterLevel])
                        locs[z[0]][z[1]].queuedWaterLevel = waterLevel
                        locs[z[0]][z[1]].queuedForSeason = True
                        
    print("water level bugs seem to be present")

#freezes some water on the map due to winter cold
def freeze(mapArray, locs):
    queue = []
    for x in range(395):
        for y in range(500):#for each pixel
            if mapArray[x, y][0:3] == (0,0,255):#if water
                canFreeze = False
                for z in findSimpNeighbors(x, y, locs):
                    color = mapArray[z[0], z[1]][0:3]
                    isLand = color != (0,0,255)
                    if isLand:
                        canFreeze = True
                if canFreeze:#if adjacent to land, enqueue
                    queue.append([x, y, 1])
                    locs[x][y].queuedForSeason = True
                    
                    
    while(queue):
        x = queue.pop(0)
        depth = x[2]
        mapArray[x[0], x[1]] = (0, 255, 255, 255)#FREEZE
        locs[x[0]][x[1]].rate = colorToRate(mapArray[x[0], x[1]])
        if depth<7:
            for z in findSimpNeighbors(x[0], x[1], locs):
                color = mapArray[z[0], z[1]][0:3]
                alreadyIn = locs[z[0]][z[1]].queuedForSeason
                if (color == (0,0,255)) and not alreadyIn:
                    queue.append([z[0], z[1], depth+1])
                    locs[z[0]][z[1]].queuedForSeason = True

#calculates the minimum cost to move from loc1 to loc2
def calcHeur(loc1, loc2):
    dist = math.sqrt(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2))
    climb = max(0, loc2.z - loc1.z)#if climbing higher than goal, penalize
    return dist + climb

#calculates the cost of moving through loc2, from loc1
def calcCost(loc1, loc2):
    dist = math.sqrt(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2))
    climb = max(0, loc2.z - loc1.z)#if climbing up, penalize
    terrain = loc2.rate
    return (dist + climb) * terrain

#take an RBG repesentation of color, and return the relative rate of movement
def colorToRate(color):
    rate = 0.0
    if color[0:3] == (248,148,18):#Open land
        rate = 1.0
    elif color[0:3] == (255,192,0):#Rough meadow
        rate = 1.2
    elif color[0:3] == (255,255,255):#Easy movement forest
        rate = 1.1
    elif color[0:3] == (2,208,60):#Slow run forest
        rate = 1.2
    elif color[0:3] == (2,136,40):#Walk forest
        rate = 1.30
    elif color[0:3] == (5,73,24):#Impassible vegetation
        rate = 100.0
    elif color[0:3] == (0,0,255):#Lake/Swamp/Marsh
        rate = 5.0
    elif color[0:3] == (71,51,3):#Paved road
        rate = 1.0
    elif color[0:3] == (0,0,0):#Footpath
        rate = 1.0
    elif color[0:3] == (100,100,100):#Footpath covered in leaves
        rate = 1.0
    elif color[0:3] == (0, 255, 255):#ice
        rate = 1.5
    elif color[0:3] == (100, 50, 0):#floodwaters
        rate = 2.0
    elif color[0:3] == (205,0,101):#out of bound
        rate = 9999
        
    return rate

#paints the path onto the outpt image
def paintPath(mapArray, path, goals, locs):
    for x in path:
        mapArray[x[0], x[1]] = (153,50,204, 255)
        neighbors = findSimpNeighbors(x[0], x[1], locs)#path
        for y in neighbors:
            if mapArray[y[0], y[1]] != (153,50,204, 255):#surrounding tiles for readability
                mapArray[y[0], y[1]] = (153,150,204, 255)
    for x in goals:#goal locations for readability
        neighbors = findSimpNeighbors(x[0], x[1], locs)
        for y in neighbors:
            mapArray[y[0], y[1]] =(255, 255, 30, 255)
    
#function to sort nodes by lowest h+g
def sortFunc(loc):
    return loc.bestDist + loc.heur

#performs an A* search to find the optimal path
def findPath(goals, locs, goalNum):
    currGoal = goals[goalNum]
    firstLoc = locs[goals[goalNum-1][0]][goals[goalNum-1][1]]
    firstLoc.bestDist = 0
    firstLoc.bestParent = None
    firstLoc.usedGoal = goalNum
    goalLoc = locs[currGoal[0]][currGoal[1]]
    considering = [firstLoc]
    while(considering):
        currLoc = considering.pop(0)
        if(calcHeur(currLoc, goalLoc) == 0):
            break#now need to produce a coordinate list path to return
        neighbors = findNeighbors(currLoc, locs)
        for x in neighbors:
            xDist = calcCost(currLoc, x)
            xHeur = calcHeur(x, goalLoc)
            if(x.bestDist <= xDist + currLoc.bestDist and x.usedGoal is goalNum):
                continue
            else:
                if(x.bestParent is None or x.usedGoal is not goalNum):
                    x.usedGoal = goalNum
                    considering.append(x)
                x.bestDist = xDist + currLoc.bestDist
                x.heur = xHeur
                x.bestParent = [currLoc.xPix, currLoc.yPix]
                
        #sort list of solutions here
        considering.sort(key=sortFunc)
    
    
    distance = 0
    path = []
    while(currLoc):
        currX = currLoc.xPix
        currY = currLoc.yPix
        path.append([currX,currY])
        if(currLoc.bestParent is None):
            break
        parentX = currLoc.bestParent[0]
        parentY = currLoc.bestParent[1]
        parentLoc = locs[parentX][parentY]
        distance = distance + math.sqrt(pow(currLoc.x - parentLoc.x, 2) + pow(currLoc.y - parentLoc.y, 2))
        currLoc = parentLoc
    return [distance, path]

def main():
    terrainImage = sys.argv[1]
    elevationFileName = sys.argv[2]
    pathFileName = sys.argv[3]
    season = sys.argv[4]
    outFileName = sys.argv[5]
    
    elevationFile = open(elevationFileName, "r")
    pathFile = open(pathFileName, "r")
    mapImage = Image.open(terrainImage)
    mapArray = mapImage.load()
        
    elevationLine = elevationFile.readline()
    locs = [[location() for j in range(500)] for x in range(395)]
    currLine = 0
    while elevationLine:
        elevations = elevationLine.split()
        for x in range(395):
            locs[x][currLine].z = float(elevations[x])
            locs[x][currLine].x = x * pixelWidth
            locs[x][currLine].y = currLine * pixelHeight
            locs[x][currLine].rate = colorToRate(mapArray[x, currLine])
            locs[x][currLine].xPix = x
            locs[x][currLine].yPix = currLine
        elevationLine = elevationFile.readline()
        currLine +=1
    if(season == "winter"):
        freeze(mapArray, locs)
    elif(season == "spring"):
        flood(mapArray, locs)
    elif(season == "fall"):
        leafify(mapArray, locs)
    goalLine = pathFile.readline()
    goals = []#written in pixel locations
    while goalLine:
        coords = goalLine.split()
        goals.append([int(coords[0]), int(coords[1])])
        goalLine = pathFile.readline()
    
    #a* algo goes here
    totalSol = [0, []]
    for x in range(len(goals)-1):
        solution = findPath(goals, locs, x+1)
        totalSol[0] = totalSol[0] + solution[0]
        for y in solution[1]:
            totalSol[1].append(y)
    print("total distance travelled is %s"%totalSol[0])
    paintPath(mapArray, totalSol[1], goals, locs)
    mapImage.save(outFileName)


if __name__ == "__main__":
    main()