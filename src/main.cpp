#include <iostream>
#include <string>
#include <fstream>

#include "Validator.h"
#include "Algorithm.h"
#include "Input.h"

// Enumerator for selection input
enum Selection {INPUT, ALGORITHM, VERIFIER};

// Prototypes
Selection displayMenu();
void input();
void algorithm();
void verifier();


int main(int argc, char** argv) {

	// Grab selection for 
	Selection selected_option = displayMenu();

	// Run appropiate option from user input
	switch (selected_option) {
		case INPUT:
			input();
			break;
		case ALGORITHM:
			algorithm();
			break;
		case VERIFIER:
			verifier();
			break;
		default:
			std::cout << "Error has occured!" << std::endl;
	}

    
    return 0;
}

// Display Menu
Selection displayMenu() {
	int option = -1;
	while (option == -1) {
		std::cout << "\nPlease select an option(1, 2, or 3):" << std::endl;
		std::cout << "1. Create Input File" << std::endl;
		std::cout << "2. Run Algorithm" << std::endl;
		std::cout << "3. Run Verifier" << std::endl;
		std::cin >> option;

		if (option == 1) {
			return INPUT;
		} else if (option == 2) {
			return ALGORITHM;
		} else if (option == 3) {
			return VERIFIER;
		} else {
			option = -1;
		}
	}
}

// Input was selected, grab filename for the input file
void input() {

	std::string filename_input;
	std::cout << "\nPlease enter the filename for the new input file including file extension:" << std::endl << std::endl;
	std::cin >> filename_input;

	// Run input function
	// std::cout << "Generating input with filename of: " << filename_input << std::endl;
	Input input(filename_input);
}


// Algorithm was selected, grab filename of input file and output file
void algorithm() {
	bool valid_file = false;

	std::string filename_output;
	std::string filename_input;

	while (!valid_file) {
		std::cout << "\nPlease enter the filename for the input file including file extension:" << std::endl;

		std::cin >> filename_input;
		std::string full_filename = "../input_data/" + filename_input;
		std::ifstream file(full_filename);

		if (file) {
			break;
		} else {
			std::cout << "File doesn't exist! Please enter a valid file.";
		}
	}

	filename_output = "";

	while (filename_output == "") {
		std::cout << "\nPlease enter the filename for the output file including file extension:" << std::endl;
		std::cin >> filename_output;
	}

	// Run algorithm
	// std::cout << "Running algorithm on input file of: " << filename_input << " and creating output file named: " << filename_output << std::endl;
	Algorithm alg(filename_input, filename_output);
}

// Verifier was selected, grab filename and output file
void verifier() {
	bool valid_file = false;

	std::string filename_output;
	std::string filename_input;

	while (!valid_file) {
		std::cout << "Please enter the filename for the input file including file extension:" << std::endl << std::endl;

		std::cin >> filename_input;
		std::string full_filename = "input_data/" + filename_input;
		std::ifstream file(full_filename);

		if (file) {
			break;
		} else {
			std::cout << "File doesn't exist! Please enter a valid file.";
		}
	}

	valid_file = false;

	while (!valid_file) {
		std::cout << "\nPlease enter the filename for the output file including file extension or -1 for verfying input only:" << std::endl << std::endl;

		std::cin >> filename_output;

		if (filename_output == "-1") {
			break;
		}

		std::string full_filename = "../input_algorithm_output/" + filename_output;
		std::ifstream file(full_filename);

		if (file) {
			break;
		} else {
			std::cout << "File doesn't exist! Please enter a valid file.";
		}
	}

	// Run validator
	// std::cout << "Running verifier on input file of: " << filename_input << " and creating output file named: " << filename_output;
	Validator validator(filename_input, filename_output);
}


