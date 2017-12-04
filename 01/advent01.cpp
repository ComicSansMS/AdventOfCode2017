#include <captcha.hpp>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    std::ifstream fin(input_filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << input_filename << "' for reading." << std::endl;
        return 1;
    }

    std::string input;
    std::getline(fin, input);
    if(!fin) {
        std::cerr << "Unable to read input from file '" << input_filename << "'." << std::endl;
        return 1;
    }

    auto const result = solveCaptcha(parseInput(input));
    std::cout << "Result is " << result << std::endl;
    return 0;
}
