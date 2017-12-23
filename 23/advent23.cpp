#include <coprocessor.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
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

    auto const program = parseInput(*input);
    CoprocessorVm vm;
    executeProgram(vm, program);

    std::cout << "First result is: " << vm.mul_count << std::endl;

    CoprocessorVm vm2;
    vm2.registers[0] = 1;
    for(int i=0; i<8; ++i) {
        executeInstruction(vm2, program[vm2.pc]);
    }
    
    auto const lower_limit = vm2.registers[1];
    auto const upper_limit = vm2.registers[2];
    assert(upper_limit < std::numeric_limits<int>::max());
    auto const s = sieve(static_cast<int>(upper_limit) + 1);

    int res = 0;
    for(int b=lower_limit; b<=upper_limit; b+=17) {
        if(s[b] == 0) { ++res; }
    }
    std::cout << "Second result is: " << res << std::endl;

    return 0;
}
