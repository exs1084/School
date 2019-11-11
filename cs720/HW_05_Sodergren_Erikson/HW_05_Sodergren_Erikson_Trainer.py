# -*- coding: utf-8 -*-
"""
Created on Sat Mar  2 13:04:57 2019

@author: Erikson
"""
from math import log

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
    

def calcEntropy(L, target):
    numKrink = 0
    numOther = 0
    total = len(L)
    if total == 0:
        return 0
    for x in L:
        if x[target] == 1:
            numKrink += 1
        else:
            numOther += 1
    probK = numKrink / total
    probO = numOther / total
    if probK == 0:
        return -1 * probO*log(probO, 2)
    if probO == 0:
        return -1 * probK*log(probK, 2)
    return -1 * ((probK*log(probK, 2))+(probO*log(probO, 2)))

def mixEntropy(L1, L2, target):
    len1 = len(L1)
    len2 = len(L2)
    total = len1+len2
    entropy1 = calcEntropy(L1, target)
    entropy2 = calcEntropy(L2, target)
    return (entropy1*len1 + entropy2*len2)/total

def emitStub(attrib, returnRight, returnLeft, thresh):
    ret = "def decStub(entry):\n"
    ret +="\tif entry[%s] <= %s:\n"%(attrib, thresh)
    ret +="\t\treturn %s\n"%returnLeft
    ret +="\telse:\n"
    ret +="\t\treturn %s\n\n"%returnRight
    return ret

def emitClassFunc():
    ret = "def my_classifier_function(entry):\n"
    ret +="\treturn decStub(entry)\n\n"
    return ret

def makeStub(data, target, attribs):
    bestEntropy = 1
    bestSplit = 0#attrib 0
    bestThresh = 0
    bestRight = 0
    bestLeft = 0
    for x in attribs:
        L1 = []
        L2 = []
        for y in data:
            if y[x] == 0:
                L1.append(y)
            else:
                L2.append(y)
        newEntropy = mixEntropy(L1, L2, target)
        if newEntropy < bestEntropy:
            bestEntropy = newEntropy
            bestSplit = x
            bestRight = round(sum(L2[target])/len(L2))
            bestLeft = round(sum(L1[target])/len(L1))
    return emitStub(bestSplit, bestRight, bestLeft, bestThresh)
    
                
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
    targetVar = 79#chocKrinkle
    outProg = header()
    outProg += makeStub(data, targetVar, attribs)
    outProg += emitClassFunc()
    outProg += trailer()
    outProgName = "HW_05_Sodergren_Erikson_Classifier.py"
    outFile = open(outProgName, "w")
    outFile.write(outProg)

if __name__ == "__main__":
    main()