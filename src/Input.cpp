#include "Input.h"

void Input::get_input_variables() {

	// Random seed
	srand(time(NULL));

	// Obtain n
	this->n = rand() % 1000 + 1;

	// Obtain m
	this->m = rand() % 500 + 1;

}

void Input::generate_u() {

	for (int i = 1; i < this->n+1; i++) {
		this->U.push_back(i);
	}

	this->check_u = this->U;

}

void Input::generate_input() {

	// Obtain input variables
	get_input_variables();

	// Form U from n
	generate_u();

	// Append headers to filename
	appendHeaders();


	// Grab m subsets from n
	for (int i = 0; i < this->m; i++) {

		// Check if any integers at not in set, check_u becomes last set
		if (i == m - 1) {
			if (check_u.size() != 0) {
				append_subset(check_u);
			} else {
				generate_subset(i);
			}
		} else {
			generate_subset(i);
		}

	}

}

void Input::appendHeaders() {
	std::ofstream myfile (this->filename, std::ios_base::app);
	if (myfile.is_open()) {
		myfile << std::to_string(this->n) << "\n" << std::to_string(this->m) << "\n";
	    myfile.close();
	}
	else std::cout << "Unable to open file";
}

void Input::append_subset(std::vector<int> subVec) {
	std::ofstream myfile (this->filename, std::ios_base::app);
	if (myfile.is_open()) {
		std::string line = "";
		for (int i = 0; i < subVec.size(); i++) {
			line += std::to_string(subVec.at(i));
			if (i < subVec.size() - 1) {
				line += " ";
			}
		}
		int weight = rand() % 1000 + 1;
		myfile << line << "\n" << std::to_string(weight) << "\n";
	    myfile.close();
	}
	else std::cout << "Unable to open file";
}

void Input::generate_subset(int i) {

	std::vector<int>::const_iterator first_index;
	std::vector<int>::const_iterator second_index;

	// Grab random first and second index
	int rand_num = rand() % this->n;
	first_index = this->U.begin() + rand_num;
	second_index = this->U.begin() + rand_num + (rand() % (this->n - rand_num));

	// Create sub vector from first and second index
	std::vector<int> subVec(first_index, second_index);

	while (subVec.size() == 0) {
		// Grab random first and second index
		rand_num = rand() % this->n;
		first_index = this->U.begin() + rand_num;
		second_index = this->U.begin() + rand_num + (rand() % (this->n - rand_num));

		// Create sub vector from first and second index
		std::vector<int> tempVec(first_index, second_index);
		subVec = tempVec;
	}

	// Remove element from check vector
	for (int j = 0; j < subVec.size(); j++) {
		if (std::find(check_u.begin(), check_u.end(), subVec.at(j)) != check_u.end()) {
			check_u.erase(std::remove(check_u.begin(), check_u.end(), subVec.at(j)), check_u.end());
		}
	}

	// Write subset to file
	append_subset(subVec);
}