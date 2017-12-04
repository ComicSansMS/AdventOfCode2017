#include <captcha.hpp>

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
    char const* input01_filename = "input01";
    char const* input02_filename = "input02";
    if(argc == 3) {
        input01_filename = argv[1];
        input02_filename = argv[2];
    }

    auto input01 = readInput(input01_filename);
    auto input02 = readInput(input02_filename);

    if(!input01 || !input02) {
        return 1;
    }

    auto const result01 = solveCaptcha(parseInput(*input01));
    std::cout << "First result is " << result01 << std::endl;

    auto const result02 = solveCaptcha2(parseInput(*input02));
    std::cout << "Second result is " << result02 << std::endl;
    return 0;
}
