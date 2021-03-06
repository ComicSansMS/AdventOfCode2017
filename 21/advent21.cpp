#include <fractal.hpp>

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

    auto const rules = parseInput(*input);
    Canvas c;

    for(int i=0; i<5; ++i) {
        c = c.applyRules(rules);
    }
    std::cout << "First result is " << c.pegCount() << std::endl;

    for(int i=0; i<13; ++i) {
        c = c.applyRules(rules);
    }
    std::cout << "Second result is " << c.pegCount() << std::endl;

    return 0;
}
