#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <set>

#define     COMPUTATION_LIMIT       1000000 // How many computations to perform before giving up :(

#ifndef ALGORITHM_H
#define ALGORITHM_H

/**
 * @brief Datatype that specifies the problem we are trying to solve
 * 
 */
struct Problem
{
    std::vector<std::vector<int>> sets; ///< All possible sets that we can choose from
    std::vector<int> weights; ///< Weights for each set (corresponding to the same indexes)
    unsigned int U; ///< Maximum value of the set we are trying to cover
};

/**
 * @brief Datatype that holds the solution to the problem
 * 
 */
struct Solution
{
    std::vector<int> set_indexes; ///< Indexes of the sets in our solution
    int weight; ///< The sum of the weights of the sets used
};


class Algorithm
{

public:
    Algorithm(std::string input_filename, std::string output_filename);
    static Problem readProblem(std::string filename);
    static bool covers(std::vector<std::vector<int>>& sets, int U);
    static void printProblemStats(Problem problem);

private:

    static std::vector<int> delimit(std::string line);

    void writeSolution();

    void solveNarrowByWeightRatio();

    void solveRandomWalk();
    Solution grabSolution(int max_weight);
    bool valid_entry(Solution possible_solution, int rand_int, std::set<int> possible_set);
    int find_set(int integer);

    Problem problem;
    Solution solution;
    std::string outputFilename;
};


#endif