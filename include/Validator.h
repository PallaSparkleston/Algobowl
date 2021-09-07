

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <map>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

class Validator
{
	public:

		Validator(std::string data, std::string algorithm) {
			this->input_data_filename = "input_data/" + data;
			this->input_algorithm_filename = algorithm;

			readInput();

			if (algorithm != "-1") {
				// run read output
			}
		}

		void readInput();

		void readOutput(string);

		bool validate();

	private:

		std::string input_data_filename;
		std::string input_algorithm_filename;

		int m;
		int n;

		std::map<int, std::pair<std::vector<int>, int>> input;

		std::vector<int> outputIndexes;

		int weight;

		std::set<int> union_sets;

		void insertSet(std::string line);


};

#endif