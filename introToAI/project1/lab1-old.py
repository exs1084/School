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

#adds leafy terrain to map due to fall
def leafify(mapArray):
    print("leafify is unimplemented")
    return mapArray

#adds water to map due to spring floods
def flood(mapArray):
    print("flood is unimplemented")
    return mapArray

#freezes some water on the map due to winter cold
def freeze(mapArray):
    print("freeze is unimplemented")
    return mapArray

#calculates the minimum cost to move from loc1 to loc2
def calcHeur(loc1, loc2):
    dist = math.sqrt(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2))
    climb = max(0, loc2.z - loc1.z)#if climbing up, penalize
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
        rate = 1.05
    elif color[0:3] == (2,208,60):#Slow run forest
        rate = 1.15
    elif color[0:3] == (2,136,40):#Walk forest
        rate = 1.30
    elif color[0:3] == (5,73,24):#Impassible vegetation
        rate = 100.0
    elif color[0:3] == (0,0,255):#Lake/Swamp/Marsh
        rate = 2.0
    elif color[0:3] == (71,51,3):#Paved road
        rate = 1.0
    elif color[0:3] == (0,0,0):#Footpath
        rate = 1.05
    elif color[0:3] == (0, 255, 255):#ice
        rate = 1.4
    elif color[0:3] == (0, 0, 150):#floodwaters
        rate = 2.0
        
    return rate

#paints the path onto the outpt image
def paintPath(mapArray, path):
    for x in path:
        mapArray[x[0], x[1]] = (153,50,204, 255)
        #    mapArray[x, 100] = (255, 0, 0, 255)
    return mapArray

#finds neighbors of a location
def findNeighbors(source, locs):
    ret = []
    ret.append([source[0], source[1]+1])
    ret.append([source[0], source[1]-1])
    
    ret.append([source[0]-1, source[1]+1])
    ret.append([source[0]-1, source[1]])
    ret.append([source[0]-1, source[1]-1])
    
    ret.append([source[0]+1, source[1]+1])
    ret.append([source[0]+1, source[1]])
    ret.append([source[0]+1, source[1]-1])
    
    for x in ret:
        if(x[0]<0 or x[0]>len(locs) or x[1]<0 or x[1]>len(locs[0])):
            ret.remove(x)
    return ret
    

def sortFunc(solution):
    return solution[0] + solution[1]

#performs an A* search to find the optimal path
def findPath(goals, locs):
    path = [goals[0]]#final path of coordinates, starts as the first location
    currGoal = goals[1]
    goalToFind = 1
    goalLoc = locs[currGoal[0]][currGoal[1]]
    heur = calcHeur(locs[goals[0][0]][goals[0][1]], locs[goals[1][0]][goals[1][1]])
    solution = [0, heur, path]#true distance, heuristic, path so far
    solList = [solution]
    i=0
    while(solList):
        solution = solList.pop(0)
        #i=i+1
        #if(i>100):
        #    return solution[2]
        for x in solList:
            if(solution[2][-1] == x[2][-1]):#remove other potentials that took longer to reach a location
                solList.remove(x)
        neighbors = findNeighbors(solution[2][-1], locs)
        oldLoc = locs[solution[2][-1][0]][solution[2][-1][1]]
        if(calcHeur(oldLoc, goalLoc) == 0):
            solList = []
            print("goal %s found"%currGoal)
            goalToFind+=1
            if(goalToFind >= len(goals)):
                print("all goals found")
                break#we are done
            else:
                currGoal = goals[goalToFind]
                goalLoc = locs[currGoal[0]][currGoal[1]]
        for x in neighbors:
            #copy, calculate distance
            newSol = deepcopy(solution)
            newSol[2].append(x)
            newLoc = locs[x[0]][x[1]]
            newSol[1] = calcHeur(newLoc, goalLoc)
            newSol[0] +=calcCost(oldLoc, newLoc)
            solList.append(newSol)
        #sort list of solutions here
        solList.sort(key=sortFunc)
    
    
    print("total distance of the path is %s meters"%solution[0])
    return solution[2]

def main():
    #terrainImage = sys.argv[1]
    #elevationFileName = sys.argv[2]
    #pathFileName = sys.argv[3]
    #season = sys.argv[4]
    #outFileName = sys.argv[5]
    terrainImage = "normal/terrain.png"
    elevationFileName = "normal/mpp.txt"
    pathFileName = "normal/brown2.txt"
    season = "summer"
    outFileName = "normal/out.png"
    
    elevationFile = open(elevationFileName, "r")
    pathFile = open(pathFileName, "r")
    mapImage = Image.open(terrainImage)
    mapArray = mapImage.load()
    if(season is "winter"):
        mapArray = freeze(mapArray)
    elif(season is "spring"):
        mapArray = flood(mapArray)
    elif(season is "fall"):
        mapArray = leafify(mapArray)
        
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
        elevationLine = elevationFile.readline()
        currLine +=1
    
    goalLine = pathFile.readline()
    goals = []#written in pixel locations
    while goalLine:
        print("goalLine: %s"%goalLine)
        coords = goalLine.split()
        goals.append([int(coords[0]), int(coords[1])])
        goalLine = pathFile.readline()
    
    #a* algo goes here
    solutionPath = findPath(goals, locs)
    
    mapArray = paintPath(mapArray, solutionPath)
    mapImage.save(outFileName)


if __name__ == "__main__":
    main()