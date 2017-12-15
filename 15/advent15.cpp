#include <dueling_generators.hpp>

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

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return sstr.str();
}

int judge(Generators gen)
{
    int const count = 40'000'000;
    int total_matches = 0;
    for(int i=0; i<count; ++i) {
        generate(gen);
        if(does_match(gen)) { ++total_matches; }
    }
    return total_matches;
}

int judge_picky(Generators gen)
{
    int const count = 5'000'000;
    int total_matches = 0;
    for(int i=0; i<count; ++i) {
        generate_picky(gen);
        if(does_match(gen)) { ++total_matches; }
    }
    return total_matches;
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

    auto gen = parseInput(*input);
    std::cout << "First result is " << judge(gen) << std::endl;
    std::cout << "Second result is " << judge_picky(gen) << std::endl;

    return 0;
}
