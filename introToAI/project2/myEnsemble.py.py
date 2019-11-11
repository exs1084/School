from train import lineToRecord
from math import inf

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

def my_classifier_function(entry):
	sum0 = 0
	sum1 = 0
	if entry[7] <= 0:
		sum0 += 1.462397997898956
	else:
		sum1 += 1.462397997898956

	if entry[10] <= 0:
		sum0 += 1.447158031342219
	else:
		sum1 += 1.447158031342219

	if entry[9] <= 0:
		sum1 += 1.3304137733491905
	else:
		sum0 += 1.3304137733491905

	if sum0 >= sum1:
		return 0
	else:
		return 1
if __name__ == '__main__':
	main()