#include <particles.hpp>

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

    auto particles = parseInput(*input);
    std::size_t last_closest = 0;
    for(int cycles_unchanged=0; cycles_unchanged<1000; ++cycles_unchanged) {
        auto closest = simulate(particles);
        if(closest != last_closest) {
            last_closest = closest;
            cycles_unchanged = 0;
        }
    }
    std::cout << "First result is " << last_closest << std::endl;

    particles = parseInput(*input);
    std::size_t last_size = 0;
    for(int cycles_unchanged=0; cycles_unchanged<100; ++cycles_unchanged) {
        simulate(particles);
        resolveCollisions(particles);
        if(particles.size() != last_size) {
            last_size = particles.size();
            cycles_unchanged = 0;
        }
    }
    std::cout << "Second result is "<< last_size << std::endl;

    return 0;
}
