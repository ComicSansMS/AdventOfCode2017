#include <jumper.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>

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

    auto tape1 = parseInput(*input);
    std::cout << "First result is: " << runJumperVM(tape1) << std::endl;

    auto tape2 = parseInput(*input);
    std::cout << "Second result is: " << runStrangeJumperVM(tape2) << std::endl;

    return 0;
}
