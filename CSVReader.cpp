#include "CSVReader.h"

CSVReader::CSVReader(std::string& file) {
	fileName = file;
}

bool CSVReader::readCSV() {
	std::ifstream file(fileName);
	try {
		if (!file.is_open()) throw std::runtime_error("Unable to open the file!");
	}
	catch (std::runtime_error & e) {
		std::cout << e.what();
		return 1;
	}
	std::pair <std::string, std::pair <int, int>> pairTemp;
	std::string strLine;
	std::string strCell;
	char c;
	while (getline(file, strLine)) {
		std::stringstream  streamLine(strLine);
		std::vector <std::string> vecTemp;
		colNum = 0;
		while (streamLine.get(c)) {
			if (c == '\r') break;
			strCell.push_back(c);
			if (c == '=') {
				pairTemp.second.first = strNum;
				pairTemp.second.second = colNum;
				cells_to_calc.push_back(pairTemp);
			}
			if (streamLine.peek() == ',') {
				vecTemp.push_back(strCell);
				colNum++;
				streamLine.ignore();
				strCell.clear();
			}
		}
		strNum++;
		if (!strCell.empty() && colNum != 0) {
			vecTemp.push_back(strCell);
			colNum++;
			strCell.clear();
		}
		data.push_back(vecTemp);
	}
	try {
		if (colNum == 0) throw std::runtime_error("Wrong format of table!");
		else if (colNum != (int)data[0].size() || strNum != (int)data.size()) throw std::runtime_error("Wrong format of table!");
	}
	catch (std::runtime_error & e) {
		std::cout << e.what();
		return 1;
	}
	data[0][0] = "";
	if (!cells_to_calc.empty()) {
		setCalcCells();
		calculateCells();
	}
	return 0;
}

void CSVReader::setCalcCells() {
	std::string strTemp;
	for (int i = 1; i < strNum; ++i) {
		for (int j = 1; j < colNum; ++j) {
			strTemp.append(data[0][j]);
			strTemp.append(data[i][0]);
			mapData.insert(std::pair<std::string, std::string>(strTemp, data[i][j]));
			for (size_t k = 0; k < cells_to_calc.size(); ++k) {
				if ((i == cells_to_calc[k].second.first) && (j == cells_to_calc[k].second.second)) {
					cells_to_calc[k].first = strTemp;
				}
			}
			strTemp.clear();
		}
	}
}

void CSVReader::calculateCells() {
	std::string calcstr;
	std::size_t opPos;
	char op;
	std::string str_first_num;
	std::string str_second_num;
	std::string strResult;
	std::map<std::string, std::string>::iterator it = mapData.begin();
	char ops[] = "+-*/";
	int firstNum = 0;
	int secondNum = 0;
	int result = 0;
	for (size_t i = 0; i < cells_to_calc.size(); ++i) {
		it = mapData.find(cells_to_calc[i].first);
		if (it != mapData.end()) {
			calcstr = it->second;
			for (char* ptrops = ops; *ptrops != '\0'; ++ptrops) {
				opPos = calcstr.find(*ptrops);
				if (opPos != calcstr.npos) {
					op = *ptrops;
					str_first_num = calcstr.substr(1, opPos - 1);
					str_second_num = calcstr.substr(opPos + 1);
					std::map<std::string, std::string>::iterator it_first_num = mapData.find(str_first_num);
					try {
						if (it_first_num == mapData.end()) throw std::runtime_error("Wrong address at first operand in cell: ");
					}
					catch (std::runtime_error& e) {
						std::cout << e.what() << it->first << std::endl;
						break;
					}
					std::map<std::string, std::string>::iterator it_second_num = mapData.find(str_second_num);
					try {
						if (it_second_num == mapData.end()) throw std::runtime_error("Wrong address at second operand in cell: ");
					}
					catch (std::runtime_error & e) {
						std::cout << e.what() << it->first << std::endl;
						break;
					}
					try {
						firstNum = stoi(it_first_num->second);
					}
					catch (std::invalid_argument & e) {
						std::cout << "Incorrect data in cell: " << it_first_num->first << std::endl;
						break;
					}
					try {
						secondNum = stoi(it_second_num->second);
					}
					catch (std::invalid_argument & e) {
						std::cout << "Incorrect data in cell: " << it_second_num->first << std::endl;
						break;
					}
					switch (op)
					{
					case '+':
						result = firstNum + secondNum;
						break;
					case '-':
						result = firstNum - secondNum;
						break;
					case '*':
						result = firstNum * secondNum;
						break;
					case '/':
						try {
							if (secondNum == 0) {
								throw std::runtime_error("Division by zero! In cell: ");
							}
							else {
								result = firstNum / secondNum;
							}
						}
						catch (std::runtime_error & e) {
							std::cout << e.what() << it->first << std::endl;
						}
						break;
					default:
						break;
					}
					if (op != '/' && secondNum != 0) {
						strResult = std::to_string(result);
						it->second = strResult;
						data[cells_to_calc[i].second.first][cells_to_calc[i].second.second] = strResult;
						result = 0;
						strResult.clear();
					}
				}
			}
		}
	}
}

void CSVReader::printResult() {
	for (int i = 0; i < strNum; ++i) {
		for (int j = 0; j < colNum; ++j) {
			std::cout << data[i][j];
			if (j < colNum - 1) std::cout << ",";
		}
		std::cout << std::endl;
	}
}