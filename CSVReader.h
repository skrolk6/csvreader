#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

class CSVReader {
	public:
		std::vector <std::vector <std::string>> data;
		std::vector <std::pair <std::string, std::pair <int, int>>> cells_to_calc;
		std::map <std::string, std::string> mapData;
		std::string fileName;
		int strNum = 0;
		int	colNum = 0;

		CSVReader() {};
		CSVReader(std::string& file);
		bool readCSV();
		void setCalcCells();
		void calculateCells();
		void printResult();

};