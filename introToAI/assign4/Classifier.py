def main():
	fileName = 'Dtree-data.txt'
	dataFile = open(fileName, 'r')#open file
	data = list()
	line = dataFile.readline()
	while line:#read data
		entry = []
		for x in line.strip().split(' '):
			if x == 'False':
				entry.append(0)
			else:
				entry.append(1)
		data.append(my_classifier_function(entry))
		line = dataFile.readline()
	outFileName = 'Classifications.csv'
	outFile = open(outFileName, 'w')
	for x in data:
		outFile.write(str(x) + '\n')

def decTree(entry):
	if entry[3] <= 0:
		return 0
	else:
		return 1
def my_classifier_function(entry):
	return decTree(entry)

if __name__ == '__main__':
	main()