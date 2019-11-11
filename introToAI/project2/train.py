# -*- coding: utf-8 -*-
"""
Created on Mon Apr 22 12:05:59 2019

@author: Erikson
"""

import sys
from TreeTrainer import treeMain
from BoostTrainer import boostMain
maxDepth = 3
maxStubs = 3

def lineToRecord(line, version):#convert an input line into a list of binary features
    if version == 2:
        line = "??|" + line
    ret = [-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    stripped = line[3:].lower().strip().split(' ')
    for x in range(len(stripped)):
        stripped[x] = stripped[x].strip(""".,(){}[]?!$'" """)
    if line[0:2] == 'en':
        ret[0] = 0
    else:
        ret[0] = 1
    for word in stripped:
        if 'ijk' in word:
            ret[1] = 1
        if word[-2:] == 'en':
            ret[2] = 1
        if "'re" in word:
            ret[3] = 1
        if "'nt" in word:
            ret[4] = 1
        if 'aa' in word:
            ret[5] = 1
        if 'q' in word:
            ret[6] = 1
        if 'ij' in word:
            ret[7] = 1
        if 'ee' in word:
            ret[8] = 1
        if 'the' == word:
            ret[9] = 1
        if 'de' == word:
            ret[10] = 1
    return ret

def main(fileName, outProgName, trainType):
    dataFile = open(fileName, "r", encoding='utf8')#open file
    outFile = open(outProgName+".py", "w")
    data = list()
    line = dataFile.readline()
    while line:#read data
        entry = lineToRecord(line, 1)
        data.append(entry)
        line = dataFile.readline()
    attribs = [x for x in range(len(data[0]))]
    attribs = attribs[1:]#remove goal col from list of attributes we can split on
    targetVar = 0
    if trainType == "dt":
        outProg = treeMain(data, targetVar, attribs, maxDepth)
        outFile.write(outProg)
    else:
        outProg = boostMain(data, targetVar, attribs, maxStubs)
        outFile.write(outProg)
    outFile.close()
    dataFile.close()

if __name__ == "__main__":
    args = sys.argv
    #args = ["train.py", "myTrain.txt", "myEnsemble.py", "ad"]
    main(args[1], args[2], args[3])