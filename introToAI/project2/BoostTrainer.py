# -*- coding: utf-8 -*-
"""
Created on Sat Mar  2 13:04:57 2019

@author: Erikson
"""
from math import log, inf

def header():
    ret = "from train import lineToRecord\n"
    ret +="from math import inf\n\n"
    ret +="def main(fileName):\n"
    ret +="\tdataFile = open(fileName, 'r', encoding='utf8')#open file\n"
    ret +="\tdata = list()\n"
    ret +="\tline = dataFile.readline()\n"
    ret +="\twhile line:#read data\n"
    ret +="\t\tentry = lineToRecord(line, 2)\n"
    ret +="\t\tresult = (my_classifier_function(entry))\n"
    ret +="\t\tif result == 0:\n"
    ret +="\t\t\tdata.append('english')\n"
    ret +="\t\telse:\n"
    ret +="\t\t\tdata.append('dutch')\n"
    ret +="\t\tline = dataFile.readline()\n"
    ret +="\toutFileName = 'Classifications.csv'\n"
    ret +="\toutFile = open(outFileName, 'w')\n"
    ret +="\tfor x in data:\n"
    ret +="\t\toutFile.write(str(x) + '\\n')\n"
    ret +="\t\tprint('%s'%x)\n\n"
    return ret


def trailer():
    return "if __name__ == '__main__':\n\tmain()"
    
def findContents(L, target):#sum up number of each class in a list
    numTarg = 0
    numOther = 0
    total = len(L)
    if total == 0:
        return 0
    for x in L:
        if x[target] == 1:
            numTarg += x[-1]
        else:
            numOther += x[-1]
    return [numTarg, numOther]

def findPurity(L, target):#program stops recursing early if a list is >95% one class
    total = len(L)
    if total == 0:
        return 1
    contents = findContents(L, target)
    return max(contents[0]/total, contents[1]/total)

def majorityClass(L, target):
    contents = findContents(L, target)
    if contents[0] > contents[1]:
        return 1
    else:
        return 0
    
def emitStub(attrib, returnRight, returnLeft, thresh, stubWeight):#legacy func
    ret ="\tif entry[%s] <= %s:\n"%(attrib, thresh)
    ret +="\t\tsum%s += %s\n"%(returnLeft, stubWeight)
    ret +="\telse:\n"
    ret +="\t\tsum%s += %s\n\n"%(returnRight, stubWeight)
    return ret

def findError(bestLeft, bestRight, classLeft, classRight, target):
    error = 0
    for x in bestLeft:
        if x[target] != classLeft:
            error += x[-1]
    for x in bestRight:
        if x[target] != classRight:
            error += x[-1]
    return error
            
def reWeight(bestLeft, bestRight, classLeft, classRight, target, error):#return new data list
    for x in bestLeft:
        if x[target] == classLeft:
            x[-1] *= error/(1-error)
    for x in bestRight:
        if x[target] == classRight:
            x[-1] *=error/(1-error)
    return bestLeft + bestRight

def normalizeWeight(data):
    weightSum = 0
    for x in data:
        weightSum += x[-1]
    for x in data:
        x[-1] /= weightSum

def makeTree(data, target, attribs):
    bestCoeff = 1
    bestSplit = 0#attrib 0
    bestThresh = 0
    bestRight = []
    bestLeft = []
    #for each atttribute
    for x in attribs:
        L1 = []
        L2 = []
        #sort data into 2 sides
        for y in data:
            if y[x] == 0:
                L1.append(y)
            else:
                L2.append(y)
        newCoeff = mixEntropy(L1, L2, target)#MINIMIZING mixed entropy is functionally identical to maximizing gain
        if newCoeff < bestCoeff:
            bestCoeff = newCoeff
            bestSplit = x
            bestRight = L2
            bestLeft = L1

    classLeft = majorityClass(bestLeft, target)
    classRight = majorityClass(bestRight, target)
    error = findError(bestLeft, bestRight, classLeft, classRight, target)
    if error == 0:#error of zero means this stub answers perfectly
        return [bestSplit, classRight, classLeft, bestThresh, inf]
    data = reWeight(bestLeft, bestRight, classLeft, classRight, target, error)
    normalizeWeight(data)
    stubWeight = log((1-error)/error, 10)
    #calc error and stubweight and new record weights
    return [bestSplit, classRight, classLeft, bestThresh, stubWeight]
    

def calcEntropy(L, target):
    numTarg = 0
    numOther = 0
    total = len(L)
    if total == 0:
        return 0
    for x in L:
        if x[target] == 1:
            numTarg += x[-1]
        else:
            numOther += x[-1]
    total = numTarg + numOther
    probYes = numTarg / total
    probNo = numOther / total
    if probYes == 0:
        return -1 * probNo*log(probNo, 2)
    if probNo == 0:
        return -1 * probYes*log(probYes, 2)
    return -1 * ((probYes*log(probYes, 2))+(probNo*log(probNo, 2)))

def mixEntropy(L1, L2, target):
    len1 = len(L1)
    len2 = len(L2)
    total = len1+len2
    entropy1 = calcEntropy(L1, target)
    entropy2 = calcEntropy(L2, target)
    return (entropy1*len1 + entropy2*len2)/total
       
def boostMain(data, targetVar, attribs, maxStubs):
    for record in data:
        record.append(1/len(data))#add weights
    outProg = header()
    outProg += "def my_classifier_function(entry):\n"
    outProg += "\tsum0 = 0\n"
    outProg += "\tsum1 = 0\n"
    #loop around here
    stubs = 0
    while stubs < maxStubs:
        stub = makeTree(data, targetVar, attribs)
        outProg += emitStub(stub[0], stub[1], stub[2], stub[3], stub[4])
        if stub[-1] == inf:
            break
        stubs += 1
    
    outProg += "\tif sum0 >= sum1:\n"
    outProg += "\t\treturn 0\n"
    outProg += "\telse:\n"
    outProg += "\t\treturn 1\n"
    outProg += trailer()
    return outProg