def main():
	fileName = 'Data_Population_Survey_as_Binary_VALIDATION_DATA_v800.csv'
	dataFile = open(fileName, 'r')#open file
	data = list()
	line = dataFile.readline()
	line = dataFile.readline()#skip the var name headers in each column
	while line:#read data
		entry = [round(float(x)) for x in line.strip().split(',')]
		data.append(my_classifier_function(entry))
		line = dataFile.readline()
	outFileName = 'HW_06_LastName_FirstName_MyClassifications.csv'
	outFile = open(outFileName, 'w')
	for x in data:
		outFile.write(str(x) + '\n')

def decStub(entry):
	if entry[67] <= 0:
		return 0
	else:
		return 0

def my_classifier_function(entry):
	return decStub(entry)

if __name__ == '__main__':
	main()