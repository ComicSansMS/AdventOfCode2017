#include <knot_hash.hpp>

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

    StringCircle sc = buildInitialCircle();
    auto const lengths = parseLengths(*input);
    KnotHashState state{ 0, 0 };
    knotHashSingleRound(sc, lengths, state);

    std::cout << "First result is: " << (sc[0] * sc[1]) << std::endl;

    std::cout << "Second result is: " << knotHash(*input) << std::endl;

    return 0;
}
