#include <bridge_builder.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
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
    auto const ports = parseInput(*input);
    auto const bridges = getBridges(ports);

    std::cout << "First result is: " << getStrongestStrength(bridges) << std::endl;

    std::cout << "Second result is: " << getLongestStrongestStrength(bridges) << std::endl;

    return 0;
}
