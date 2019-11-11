usage instructions:
training: python3 train.py [training data file] [hypothesis output name] [dt for dectree, anythign else for adaboost]
		outputs: [hypothesis output name].py

testing: python3 predict.py [hypothesis name excluding '.py'] [testing data file]
		OUTPUT: predictions in order of sentences in the input file. also outputs to "Classifications.csv"


training and testing data files must be encoded with utf8! 
if using other encoding, must change the arguments of the file open calls in train.py