#include <Algorithm.h>

/**
 * @brief Construct a new Algorithm:: Algorithm object
 * 
 * @param input_filename Filename to read the input file from
 * @param output_filename Filename to write the output to
 */
Algorithm::Algorithm(std::string input_filename, std::string output_filename)
{
    srand(time(NULL));
    outputFilename = output_filename;
    problem = readProblem(input_filename);
    printProblemStats(problem);
    // solveNarrowByWeightRatio();
    solveRandomWalk();
    // writeSolution();
}

/**
 * @brief Takes a string of space delimited ints and returns the vector of ints
 * 
 * @param line String of space delimited ints
 * @return std::vector<int> Ints found in the string (in order of found)
 */
std::vector<int> Algorithm::delimit(std::string line)
{
    std::vector<int> set;
    std::string string_value = "";

    for (char c : line)
    {
        if (c == ' ')
        {
            set.push_back(std::stoi(string_value));
            string_value = "";
        }else
        {
            string_value += c;
        }
    }

    if (string_value != " " && string_value != "")
    {
        set.push_back(std::stoi(string_value));
    }
    
    return set;
}

/**
 * @brief Reads the specially formatted input file and returns a Problem struct containing its information
 * 
 * @param filename File to read the input from
 * @return Problem Problem specified by the input file
 */
Problem Algorithm::readProblem(std::string filename)
{
    size_t num_sets;
    std::string line;
    Problem problem;

    std::ifstream in("./input_data/" + filename);

    // Checking to make sure we can open the file
    if (!in.is_open())
    {
        printf("Error opening \"%s\"\n", filename.c_str());
        return problem;
    }

    // Reading U
    getline(in, line);
    problem.U = std::stoi(line);

    // Reading the number of sets
    getline(in, line);
    num_sets = std::stoi(line);
    
    // Reading each set
    for (size_t i = 0; i < num_sets; i++)
    {
        if (!getline(in, line))
        {
            printf("Input file \"%s\" formatted wrong\n", filename.c_str());
            break;
        }

        problem.sets.push_back(delimit(line));

        getline(in, line);
        problem.weights.push_back(std::stoi(line));
    }

    in.close();

    return problem;
}

/**
 * @brief Print statistics about the problem
 * 
 * @param problem Problem for which to print statistics
 */
void Algorithm::printProblemStats(Problem problem)
{
    size_t num_elements = 0;
    for (auto set : problem.sets)
    {
        num_elements += set.size();
    }

    std::cout << std::endl;
    std::cout << "Problem Statistics" << std::endl;
    std::cout << "U: " << problem.U << std::endl;
    std::cout << "Weight range: ["
        << *std::min_element(problem.weights.begin(), problem.weights.end())
        << ", "
        << *std::max_element(problem.weights.begin(), problem.weights.end())
        << "]"
        << std::endl;

    std::cout << "Number of sets: " << problem.sets.size() << std::endl;
    std::cout << "Number of elements: " << num_elements << std::endl;
}

/**
 * @brief Write the solution to the output file
 * 
 */
void Algorithm::writeSolution()
{
    std::ofstream out("output/" + outputFilename, std::ios::trunc);

    out << solution.weight << '\n';

    for (size_t i = 0; i < solution.set_indexes.size(); i++)
    {
        out << solution.set_indexes[i] + 1;
        if (i != solution.set_indexes.size() - 1)
        {
            out << " ";
        }
    }
    out.close();
}

/**
 * @brief Find a solution by starting with the entire set and removing elements one by one based on the cost of each element
 * 
 * This algorithm calculates the amount of elements you get per unit of weight (size of set / weight), then it
 *  -# Removes the element with the lowest size/weight ratio
 *  -# Checks to see if the remaining sets still cover the total set
 *  -# If it does not still cover the set, replace it and try the next lowest one.
 * 
 * The algorithm terminates under two conditions
 *  -# There is no set that you could remove from the remaining sets that result in a set that still covers
 *  -# The computational limit is reached (timeout)
 * 
 */
void Algorithm::solveNarrowByWeightRatio()
{
    std::chrono::high_resolution_clock::time_point start, stop;
    std::stack<std::vector<int>> removed;
    std::vector<std::pair<float, size_t>> stw_indexes; // Size/weight and indexes
    std::vector<std::vector<int>> sets = problem.sets;
    Solution proposed_solution;


    int weight = 0;

    for (size_t i = 0; i < problem.sets.size(); i++)
    {
        stw_indexes.push_back(std::make_pair(
                (float)problem.sets[i].size() / ((float)problem.weights[i]), // weight per size (how much it costs per element)
                i)); // Index

        weight += problem.weights[i];
        proposed_solution.set_indexes.push_back(i);
    }

    proposed_solution.weight = weight;

    std::sort(stw_indexes.begin(),stw_indexes.end()); // Sort based on the weight ratio (sets that have the lowest cost will be first)
    size_t failed_attempts = 0; // Will stop it if there is no other way to take an element off that will result in a set that still covers
    size_t total_steps = 0; // Will stop it if we reach some computation limit

    
    printf("\nRunning Narrow By Weight Ratio Algorithm\n");
    start = std::chrono::high_resolution_clock::now();
    while (failed_attempts < stw_indexes.size() && total_steps != COMPUTATION_LIMIT)
    {
        for (std::pair<float, size_t> set_to_remove : stw_indexes)
        {
            removed.push(sets[set_to_remove.second]); // Save the removed element
            sets.erase(sets.begin() + set_to_remove.second); // Remove the element

            if (covers(sets, problem.U)) // See if the set still covers
            {   
                proposed_solution.set_indexes.erase(std::find( // Erase the specific index from the set of indexes
                    proposed_solution.set_indexes.begin(),
                    proposed_solution.set_indexes.end(),
                    set_to_remove.second
                ));
                proposed_solution.weight -= problem.weights[set_to_remove.second];
                failed_attempts = 0;
                break;
            }else // If it doesn't still cover, put the item back in the set 
            {
                sets.insert(sets.begin() + set_to_remove.second, removed.top());
                removed.pop();
                failed_attempts++;
            }
        }

        // Print steps (sanity check to make sure the algorithm is still running for big ones)
        if (total_steps++ % 10000 == 0)
        {
            printf("\rStep: %lu        ", total_steps);
        }
    }
    stop = std::chrono::high_resolution_clock::now();
    
    if (total_steps == COMPUTATION_LIMIT) std::cout << std::endl << "COMPUTATIONAL LIMIT REACHED\n" << std::endl;

    solution = proposed_solution;

    sets.clear();
    size_t total_elements_used = 0;
    for (int i : solution.set_indexes)
    {
        sets.push_back(problem.sets[i]);
        total_elements_used += problem.sets[i].size();
    }

    // Printing Statistics
    std::cout << std::endl << std::endl;
    std::cout << "Algorithm Statistics" << std::endl;
    std::cout << "Covers: " << ( covers(sets, problem.U) ? "yes" : "no") << std::endl;
    std::cout << "Weight: " << solution.weight << std::endl;
    std::cout << "Number of sets used: " << solution.set_indexes.size() << std::endl;
    std::cout << "Number of elements used: " << total_elements_used << std::endl;
    std::cout << "Total time: " << ((double)(stop-start).count()) / (1000.0 * 1000.0) << " s" << std::endl;
    if (total_steps != COMPUTATION_LIMIT)
    {
        std::cout << "Iterations to proposed solution: " << total_steps << std::endl;
    }
}

/**
 * @brief Determines if the sets provided cover the set that is the elements {1, 2, ..., U}
 * 
 * @param sets Proposed sets
 * @param U Max value for the set you are trying to cover
 * @return true Covers set
 * @return false Does not cover set
 */
bool Algorithm::covers(std::vector<std::vector<int>>& sets, int U)
{
    std::vector<bool> cover_set(U, false);
    bool covers = true;

    for (std::vector<int> set : sets)
    {
        for (int index : set)
        {
            if (index >= 0)
            {
                cover_set[index-1] = true;
            }
        }
    }

    for (bool c : cover_set)
    {
        covers &= c;
    }

    return covers;
}

void Algorithm::solveRandomWalk() {

    int cost = 836;
    int max_weight = -1;
    Solution best_solution = grabSolution(max_weight);
    max_weight = best_solution.weight;
    int attempt = 0;
    while(attempt < 1000) {
        Solution possible_solution = grabSolution(max_weight);
        if (possible_solution.weight == -1) {
            continue;
        }
        if (possible_solution.weight < best_solution.weight) {
            best_solution = possible_solution;
            max_weight = best_solution.weight;
        }
        if (cost != -1 && cost > best_solution.weight) {
            break;
        }
        attempt += 1;
    }
    solution = best_solution;
    writeSolution();
    return;
}

// Return valid solution from input
Solution Algorithm::grabSolution(int max_weight) {
    Solution first_solution;

    first_solution.weight = 0;
    int size_u = problem.U;
    int set_size = problem.sets.size();
    int range = size_u * 0.9;
    std::set<int> possible_set;

    // While solution doesn't contain 90% of U
    while (possible_set.size() < range) {
        int rand_int = rand() % set_size;
        if (valid_entry(first_solution, rand_int, possible_set)) {
            first_solution.set_indexes.push_back(rand_int);
            first_solution.weight += problem.weights.at(rand_int);
            if (max_weight != -1 && first_solution.weight > max_weight) {
                first_solution.weight = -1;
                return first_solution;
            }
            possible_set.insert(problem.sets.at(rand_int).begin(), problem.sets.at(rand_int).end());
        } else {
            continue;
        }
    }

    // While solution is not complete
    while (possible_set.size() != problem.U) {
        for(int i = 1; i <= problem.U; i++) {
            std::set<int>::iterator it;
            it = std::find(possible_set.begin(), possible_set.end(), i);
            if (it != possible_set.end()) {
                continue;
            } else {
                int set_int = find_set(i);
                first_solution.set_indexes.push_back(set_int);
                first_solution.weight += problem.weights.at(set_int);
                if (max_weight != -1 && first_solution.weight > max_weight) {
                    first_solution.weight = -1;
                    return first_solution;
                }
                possible_set.insert(problem.sets.at(set_int).begin(), problem.sets.at(set_int).end());
                break;
            }
        }
    }

    return first_solution;
}

// Locates set with integer in it
int Algorithm::find_set(int integer) {
    for(int i = 0; i < problem.sets.size(); i++) {
        std::vector<int>::iterator it;
        it = std::find(problem.sets.at(i).begin(), problem.sets.at(i).end(), integer);
        if (it != problem.sets.at(i).end()) {
            return i;
        } else {
            continue;
        }
    }
}

// Check if set should be inserted into possible solution
bool Algorithm::valid_entry(Solution possible_solution, int rand_int, std::set<int> possible_set) {
    std::vector<int>::iterator it;
    it = std::find(possible_solution.set_indexes.begin(), possible_solution.set_indexes.end(), rand_int);
    if (it != possible_solution.set_indexes.end()) {
        return false;
    } 
    else {
        for(int i = 0; i < this->problem.sets.at(rand_int).size(); i++) {
            if (possible_set.find(problem.sets.at(rand_int).at(i)) == possible_set.end()) {
                return true;
            } else {
                continue;
            }
        }
        return false;
    }
    return false;
}