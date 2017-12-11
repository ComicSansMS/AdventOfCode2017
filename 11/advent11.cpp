#include <hexgrid.hpp>

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

    auto const path = getPath(CubicCoordinates(), parseInput(*input));
    std::cout << "First result is: " << distanceFromOrigin(path.back()) << std::endl;
    
    auto it_max = std::max_element(begin(path), end(path),
        [](CubicCoordinates const& lhs, CubicCoordinates const& rhs) -> bool {
            return distanceFromOrigin(lhs) < distanceFromOrigin(rhs);
        });
    std::cout << "Second result is: " << distanceFromOrigin(*it_max) << std::endl;

    return 0;
}
