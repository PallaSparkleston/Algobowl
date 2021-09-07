#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <fstream>

class Input {
	private:

    	std::string filename;
    	int n;
    	int m;
    	int num_subsets = 0;

    	std::vector<int> U;
    	std::vector<int> check_u;

    	void get_input_variables();
    	void generate_u();
        void generate_input();
        void append_subset(std::vector<int> subVec);
        void generate_subset(int i);
        void appendHeaders();

	public:

		Input(std::string filename) {
			this->filename = "input_data/" + filename;
            generate_input();
		};
};


#endif