#include <memory_banks.hpp>

#include <fstream>
#include <iostream>
#include <optional>
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

    auto banks = parseInput(*input);
    auto const result1 = redistribute(banks);
    std::cout << "First result is " << result1 << std::endl;
    auto const result2 = redistribute(banks);
    std::cout << "Second result is " << result2 << std::endl;

    return 0;
}
