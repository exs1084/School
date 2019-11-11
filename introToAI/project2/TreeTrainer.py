# -*- coding: utf-8 -*-
"""
Created on Sat Mar  2 13:04:57 2019

@author: Erikson
"""
from math import log

def header():
    ret = "from train import lineToRecord\n\n"
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
            numTarg += 1
        else:
            numOther += 1
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

def emitClassFunc():
    ret = "def my_classifier_function(entry):\n"
    ret +="\treturn decTree(entry)\n\n"
    return ret

def indent(depth):#python is picky about indentation
    ret = ""
    for x in range(depth):
        ret += "\t"
    return ret

def makeTree(data, target, attribs, depth, maxDepth):
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
    
    ret = ""
    if(depth<maxDepth):
        ret += indent(depth)
        ret += "if entry[%s] <= %s:\n"%(bestSplit, bestThresh)
        if(findPurity(bestLeft, target) < 0.95):
            ret += makeTree(bestLeft, target, attribs, depth+1, maxDepth)
        else:
            ret += indent(depth+1) + "return %s\n"%majorityClass(bestLeft, target)
        ret += indent(depth) + "else:\n"
        if(findPurity(bestRight, target) < 0.95):
            ret += makeTree(bestRight, target, attribs, depth+1, maxDepth)
        else:
            ret += indent(depth+1) + "return %s\n"%majorityClass(bestRight, target)
    else:
        ret += indent(depth)
        ret += "if entry[%s] <= %s:\n"%(bestSplit, bestThresh)
        ret += indent(depth+1) + "return %s\n"%majorityClass(bestLeft, target)
        ret += indent(depth) + "else:\n"
        ret += indent(depth+1) + "return %s\n"%majorityClass(bestRight, target)
    return ret
    

def calcEntropy(L, target):
    numTarg = 0
    numOther = 0
    total = len(L)
    if total == 0:
        return 0
    for x in L:
        if x[target] == 1:
            numTarg += 1
        else:
            numOther += 1
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
       
def treeMain(data, targetVar, attribs, maxDepth):
    
    outProg = header()
    outProg += "def decTree(entry):\n"
    outProg += makeTree(data, targetVar, attribs, 1, maxDepth)
    outProg += emitClassFunc()
    outProg += trailer()
    return outProg