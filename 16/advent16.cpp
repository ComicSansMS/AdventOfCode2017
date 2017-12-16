#include <dancing_programs.hpp>

#include <algorithm>

#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << filename << "' for reading." << std::endl;
        return std::nullopt;
    }

    std::string input;
    std::getline(fin, input);
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return input;
}

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto stage = createDancingStage();
    auto dance = parseInput(*input);
    perform(stage, dance);

    std::cout << "First result is " << to_string(stage) << std::endl;

    stage = createDancingStage();
    auto const period = findPeriod(stage, dance);
    // in the general case, the period could end at a different permutation than the starting input
    // this would require treating two lengths: the prefix for getting into the cycle and then the
    // period of the cycle; but for our input here, there is no prefix phase.
    stage = createDancingStage();
    for(std::size_t i=0; i<(1'000'000'000 % period); ++i) {
        perform(stage, dance);
    }
    std::cout << "Second result is " << to_string(stage) << std::endl;

    return 0;
}
