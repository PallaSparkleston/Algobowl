#include <Validator.h>

void Validator::readInput() {
	ifstream data(this->input_data_filename);

	std::string line;

	int line_count = 0;
	int subset_count = 0;

	if (data.is_open()) {
		while(getline(data, line)) {
			if (line_count == 0) {
				this->n = std::stoi(line);
				if (this->n > 1000) {
					std::cout << "N is not less than or equal to 1000" << std::endl;
					break;
				}
			} else if (line_count == 1) {
				this->m = std::stoi(line);
				if (this->m > 500) {
					std::cout << "M is not less than or equal to 500" << std::endl;
					break;
				}
			} else {
				subset_count += 1;
				// Insert subset (line) to data structure
				insertSet(line);
				getline(data, line);
				// Insert weight (line) to data structure
			}
			line_count += 1;
		}
		data.close();
	} else {
		std::cout << "unable to open file" << std::endl;
	}

	// Check if there are m datasets
	if (this->m != subset_count) {
		std::cout << "There is an incorrect amount of subsets in data" << std::endl;
		return;
	}

	if (this->union_sets.size() != this->n) {
		std::cout << "Invalid size for n!" << std::endl;
		return;
	}

	std::cout << "Input data file is valid!" << std::endl;

}

void Validator::insertSet(std::string line) {
	std::string number = "";
	for(int i = 0; i < line.size(); i++) {
		if ((char)line[i] == ' ') {
			int set_number = std::stoi(number);
			this->union_sets.insert(set_number);
			number = "";
		} else {
			number += line[i];
		}
	}
	int set_number = std::stoi(number);
	this->union_sets.insert(set_number);
	return;
}

void Validator::readOutput(std::string fileName) {
	ifstream output(fileName);
	std::string currentSInt;
	std::string nextSInt;
	int nextInt;

	if (output.is_open()) {
		getline(output, currentSInt);
		weight = std::stoi(currentSInt);
		while (getline(output, nextSInt)) {
			nextInt = std::stoi(nextSInt);
			outputIndexes.push_back(nextInt);
		}
	} else {
		cerr << "Unable to open output file " << fileName << endl;
	}


}

bool Validator::validate() {
	int weightSum = 0;
	set<int> Union;
	set<int> outputUnion;
	vector<int> currentSet;

	for (int index:outputIndexes) {
		weightSum += input[index].second; 
	}

	if (weightSum != weight)
		return false;

	for (map<int, pair<vector<int>, int>>::iterator it = input.begin(); it != input.end(); it++) {
		currentSet = it->second.first;
		for (int i = 0; i < currentSet.size(); i++) {
			Union.insert(currentSet[i]);
		}
	}

	for (int index:outputIndexes) {
		currentSet = input[index].first;
		for (int i = 0; i < currentSet.size(); i++) {
			outputUnion.insert(currentSet[i]);
		}
	}

	if (Union != outputUnion)
		return false;

	
	return true;

}