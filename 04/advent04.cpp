#include <passphrase.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

std::optional<std::vector<std::string>> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << filename << "' for reading." << std::endl;
        return std::nullopt;
    }

    std::vector<std::string> input;
    std::string line;
    while(std::getline(fin, line)) {
        input.push_back(line);
    }
    if(!fin.eof()) {
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

    int valid_phrases = 0;
    for(auto const& passphrase : *input) {
        if(!hasDuplicates(passphrase)) {
            ++valid_phrases;
        }
    }
    std::cout << "First result is " << valid_phrases << std::endl;

    return 0;
}
