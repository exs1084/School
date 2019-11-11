import sys
import queue

def countDiff(word1, word2, wordlen):
    count=0
    for x in range(wordlen):
        if word1[x] != word2[x]:
            count = count+1

    return count

def main():
    start = sys.argv[1]#read input words
    goal = sys.argv[2]
    filename = sys.argv[3]
    if(len(start) != len(goal)):
        print("error: words not the same length")
        return
    wordLen = len(start)#record length
    queueList = []
    for i in range(wordLen-1):
        queueList.append(queue.Queue(maxsize=0))#prepare queue's. one each for possible number of letters to be wrong by in the most recent change
        #queueList[0] = 2 wrong letter, [1] = 3 wrong letters, etc. no need for 1 wrong letter, because thats a solution

    #read dictionary file and create 26 lists, one for each letter a word starts with
    dictFile = open(filename, "r")
    dictLists = []
    for x in range(26):
        dictLists.append([])
    line = dictFile.readline()
    while line:
        word = line.rstrip()
        if(len(word)==wordLen):
            dictLists[ord(line[0])-97].append(word)
        line = dictFile.readline()
    numWrong = countDiff(start, goal, wordLen)
    if(numWrong==1):
        seq = list([start])
        seq.append(goal)
        print(seq)
        return
    queueList[numWrong-2].put([start])#place starting word in the corret queue
    if(start in dictLists[ord(start[0])-97]):
        dictLists[ord(start[0])-97].remove(start)#remove starting word from dictionary

    while True:
        currSeq = None
        for seqQueue in queueList:
            if(not seqQueue.empty()):#find closest existing sequence to a solution
                currSeq = seqQueue.get()
                break
        if(currSeq==None):
            print("could not find a solution")
            return
        currWord = currSeq[-1]#grab newest word in this sequence
        mainListIdx = ord(currWord[0])-97
        for y in range(26):
            if (y is not mainListIdx):
                letter = chr(97+y)
                newWord = letter + currWord[1:]
                if(newWord in dictLists[y]):
                    newSeq = currSeq.copy()
                    newSeq.append(newWord)#create new sequence
                    numWrong = countDiff(newWord, goal, wordLen)
                    if(numWrong==1):
                        newSeq.append(goal)
                        print(newSeq)
                        return
                    queueList[numWrong-2].put(newSeq)#add new sequence to queue
                    dictLists[y].remove(newWord)#remove chosen word from dict
            #can we find a word here?

        for y in range(26):#for each letter in alphabet
            letter = chr(97+y)
            for x in range(1, wordLen):#try in each place of the word
                newWord = currWord[0:x] + letter + currWord[x+1:]
                if(newWord in dictLists[mainListIdx]):
                    newSeq = currSeq.copy()
                    newSeq.append(newWord)#create new sequence
                    numWrong = countDiff(newWord, goal, wordLen)
                    if(numWrong==1):
                        newSeq.append(goal)
                        print(newSeq)
                        return
                    queueList[numWrong-2].put(newSeq)#add new sequence to queue
                    dictLists[mainListIdx].remove(newWord)#remove chosen word from dict
                

        #
        #find all VALID words you can make from this that are not in the sequence, call wordDif, add to queue's. if any has dif=1, solve
        #for each letter in word, check if correct. then check for replacements. remove newly chosen words from the dictionary

    


if __name__ == "__main__":
    main()