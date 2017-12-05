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
    char const* input01_filename = "input01";
    char const* input02_filename = "input02";
    if(argc == 3) {
        input01_filename = argv[1];
        input02_filename = argv[2];
    }

    auto const input01 = readInput(input01_filename);
    if(!input01) {
        return 1;
    }
    auto const result01 = calculateSpreadChecksum(parseInput(*input01));
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
