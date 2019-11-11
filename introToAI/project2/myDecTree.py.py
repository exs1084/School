from train import lineToRecord

def main(fileName):
	dataFile = open(fileName, 'r', encoding='utf8')#open file
	data = list()
	line = dataFile.readline()
	while line:#read data
		entry = lineToRecord(line, 2)
		result = (my_classifier_function(entry))
		if result == 0:
			data.append('english')
		else:
			data.append('dutch')
		line = dataFile.readline()
	outFileName = 'Classifications.csv'
	outFile = open(outFileName, 'w')
	for x in data:
		outFile.write(str(x) + '\n')
		print('%s'%x)

def decTree(entry):
	if entry[7] <= 0:
		if entry[10] <= 0:
			return 0
		else:
			return 1
	else:
		return 1
def my_classifier_function(entry):
	return decTree(entry)

if __name__ == '__main__':
	main()