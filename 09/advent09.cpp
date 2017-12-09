#include <garbage_stream.hpp>

#include <algorithm>

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

    auto sm = parseStream(*input);
    if(!sm.inAcceptingState()) {
        std::cout << "Invalid input. State machine did not accept." << std::endl;
        return 1;
    }

    std::cout << "First result is " << sm.getScore() << std::endl;
    std::cout << "Second result is " << sm.getGarbageCount() << std::endl;

    return 0;
}
