#include <spreadsheeet.hpp>

#include <fstream>
#include <iostream>
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
    auto const result01 = calculateSpreadChecksum(parseInput(*input));
    std::cout << "First result is " << result01 << std::endl;
    /*
    auto const input02 = readInput(input02_filename);
    if(!input02) {
        return 1;
    }
    auto const result02 = solveCaptcha2(parseInput(*input02));
    std::cout << "Second result is " << result02 << std::endl;
    */
    return 0;
}
