# -*- coding: utf-8 -*-
"""
Created on Thu Feb 21 11:19:10 2019

@author: Erikson
"""
import random
import operator
useOp = {"+": operator.add, "-": operator.sub, "*": operator.mul, "/": operator.truediv}

def evaluate(nums, ops):
    curr = nums[0]
    for x in range(len(ops)):
        curr = useOp[ops[x]](curr, nums[x+1])
    return curr

#change the operator at an index in a list
def change(idx, op, L):
    L[idx] = op
    return L

#swaps the position of 2 elements in a list
def swap(idx1, idx2, L):
    temp = L[idx1]
    L[idx1] = L[idx2]
    L[idx2] = temp
    return L

def printEquation(nums, ops):
    string = str(nums[0])
    for x in range(len(ops)):
        string = string + ops[x] + str(nums[x+1])
    print(string)

def main():
    nums = [random.randint(1, 9) for x in range(0,100)]
    ops = [random.randint(0, 3) for x in range(0, 99)]
    target = random.randint(0, 10000)
    print("Number Set: %s"%nums)
    print("Target: %s"%target)
    #numRestart = random.sample(nums, 100)
    #opRestart = random.sample(ops, 99)
    for x in range(99):
        if ops[x] is 0:
            ops[x] = "+"
        if ops[x] == 1:
            ops[x] = "-"
        if ops[x] == 2:
            ops[x] = "*"
        if ops[x] == 3:
            ops[x] = "/"
    if(evaluate(nums, ops) == target):
        print("answer found: ", end = "")
        printEquation(nums, ops)
        return
    #restart loop
    print("***************************************")
    print("RR Iteration: 1")
    iteration = 1
    print("S0: ", end = "")
    printEquation(nums, ops)
    bestDist = abs(evaluate(nums, ops)-target)
    print("distance from target: %s\n\n"%bestDist)
    while True:
        innerBest = abs(evaluate(nums, ops)-target)
        #hill-climbing loop
        while True:
            bestChoice = innerBest + 1
            choiceOps = ops[:]
            choiceNums = nums[:]
            #for each number other than the first
            #print("start checking swaps")
            for x in range(1, 100):
                #try swapping with each number to its left
                for y in range(x):
                    tempNums = nums[:]
                    swap(x, y, tempNums)
                    tempDist = abs(evaluate(tempNums, ops)-target)
                    if(tempDist < bestChoice):
                        bestChoice = tempDist
                        choiceNums = tempNums
                        choiceOps = ops[:]
                    #calc if good
            #for each operator
            #print("start checking changes")
            for x in range(99):
                opList = ["+","-","*","/"]
                for y in opList:
                    tempOps = ops[:]
                    change(x, y, tempOps)
                    #calc if good
                    tempDist = abs(evaluate(nums, tempOps)-target)
                    if(tempDist < bestChoice):
                        bestChoice = tempDist
                        choiceOps = tempOps
                        choiceNums = nums[:]
            #update innerBest with chosen move
            #print("testing")
            if(bestChoice < innerBest):
                ops = choiceOps
                nums = choiceNums
                innerBest = bestChoice
                #update total best if necessary
                if(innerBest < bestDist):
                    print("New best equation: ")
                    printEquation(nums, ops)
                    print("distance from target: %s\n\n"%innerBest)
                    bestDist = innerBest
            else:
                #time to restart
                break
        if(bestDist == 0.0):
            print("answer found")
            printEquation(nums, ops)
            return
        #randomly select new starting state
        iteration+=1
        print("***************************************")
        print("RR Iteration: %s 	Overall Best: %s"%(iteration, bestDist))
        nums = random.sample(nums, 100)
        ops = random.sample(ops, 99)
        print("S0: ", end = "")
        printEquation(nums, ops)
        innerBest = abs(evaluate(nums, ops)-target)
        print("distance from target: %s\n\n"%innerBest)
        
    return

if __name__ == "__main__":
    main()