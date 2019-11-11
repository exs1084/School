# -*- coding: utf-8 -*-
"""
Created on Sat Mar  2 13:04:57 2019

@author: Erikson
"""
from math import log, sqrt

alpha = 0.2

def header():
    ret = "def main():\n"
    ret +="\tfileName = 'Data_Population_Survey_as_Binary_VALIDATION_DATA_v800.csv'\n"
    ret +="\tdataFile = open(fileName, 'r')#open file\n"
    ret +="\tdata = list()\n"
    ret +="\tline = dataFile.readline()\n"
    ret +="\tline = dataFile.readline()#skip the var name headers in each column\n"
    ret +="\twhile line:#read data\n"
    ret +="\t\tentry = [round(float(x)) for x in line.strip().split(',')]\n"
    ret +="\t\tdata.append(my_classifier_function(entry))\n"
    ret +="\t\tline = dataFile.readline()\n"
    ret +="\toutFileName = 'HW_06_LastName_FirstName_MyClassifications.csv'\n"
    ret +="\toutFile = open(outFileName, 'w')\n"
    ret +="\tfor x in data:\n"
    ret +="\t\toutFile.write(str(x) + '\\n')\n\n"
    return ret

def trailer():
    return "if __name__ == '__main__':\n\tmain()"
    

def findContents(L, target):
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

def findPurity(L, target):
    total = len(L)
    if total == 0:
        return 1
    contents = findContents(L, target)
    return max(contents[0]/total, contents[1]/total)

def majorityClass(L, target):
    sum1 = 0
    sum0 = 0
    for x in L:
        if x[target] == 0:
            sum0+=1
        else:
            sum1+=1
    if sum1 > sum0:
        return 1
    else:
        return 0

def bCoeff(L1, L2, target):
    content1 = findContents(L1, target)
    content2 = findContents(L2, target)
    if(content1 == 0 or content2 == 0):
        return 1
    probYes1 = content1[0]/(content1[0]+content1[1])
    probYes2 = content2[0]/(content2[0]+content2[1])
    probNo1  = content1[1]/(content1[0]+content1[1])
    probNo2  = content2[1]/(content2[0]+content2[1])
    size1 = content1[0]+content1[1]
    size2 = content2[0]+content2[1]
    regularization = 1-(min(size1, size2) / max(size1, size2))
    objective = sqrt(probYes1*probYes2) + sqrt(probNo1*probNo2)
    return (1-alpha)*objective + alpha*regularization

def emitStub(attrib, returnRight, returnLeft, thresh):
    ret ="\tif entry[%s] <= %s:\n"%(attrib, thresh)
    ret +="\t\treturn %s\n"%returnLeft
    ret +="\telse:\n"
    ret +="\t\treturn %s\n\n"%returnRight
    return ret

def emitClassFunc():
    ret = "def my_classifier_function(entry):\n"
    ret +="\treturn decTree(entry)\n\n"
    return ret

def indent(depth):
    ret = ""
    for x in range(depth):
        ret += "\t"
    return ret

def makeTree(data, target, attribs, depth):
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
        newCoeff = bCoeff(L1, L2, target)
        if newCoeff < bestCoeff:
            bestCoeff = newCoeff
            bestSplit = x
            bestRight = L2
            bestLeft = L1
    
    ret = ""
    if(depth<3):
        ret += indent(depth)
        ret += "if entry[%s] <= %s:\n"%(bestSplit, bestThresh)
        if(findPurity(bestLeft, target) < 0.95):
            ret += makeTree(bestLeft, target, attribs, depth+1)
        else:
            ret += indent(depth+1) + "return %s\n"%majorityClass(bestLeft, target)
        ret += indent(depth) + "else:\n"
        if(findPurity(bestRight, target) < 0.95):
            ret += makeTree(bestRight, target, attribs, depth+1)
        else:
            ret += indent(depth+1) + "return %s\n"%majorityClass(bestRight, target)
    else:
        ret += indent(depth)
        ret += "if entry[%s] <= %s:\n"%(bestSplit, bestThresh)
        ret += indent(depth+1) + "return %s\n"%majorityClass(bestLeft, target)
        ret += indent(depth) + "else:\n"
        ret += indent(depth+1) + "return %s\n"%majorityClass(bestRight, target)
    return ret
    
                
def main():
    fileName = "Data_Population_Survey_as_Binary_v800.csv"
    dataFile = open(fileName, "r")#open file
    data = list()
    line = dataFile.readline()
    line = dataFile.readline()#skip the var name headers in each column
    while line:#read data
        entry = [round(float(x)) for x in line.strip().split(',')]
        data.append(entry)#remove cookie columns
        line = dataFile.readline()
    attribs = [x for x in range(len(data[0]))]
    attribs = attribs[0:75] + attribs[86:]#remove cookie columns
    targetVar = 75#crumpets
    outProg = header()
    outProg += "def decTree(entry):\n"
    outProg += makeTree(data, targetVar, attribs, 1)
    outProg += emitClassFunc()
    outProg += trailer()
    outProgName = "HW_06_Sodergren_Erikson_Classifier.py"
    outFile = open(outProgName, "w")
    outFile.write(outProg)

if __name__ == "__main__":
    main()