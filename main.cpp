#include "CSVReader.h"

int main(int argc, char* argv[]) {
	std::string fileName = (argv[1]);
	CSVReader MyReader(fileName);
	if (!MyReader.readCSV()) {
		MyReader.printResult();
	}
}