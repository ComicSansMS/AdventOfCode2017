#include <duet_vm.hpp>

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
    auto const program = parseInput(*input);
    DuetVm vm;
    vm.rcv_callback = [&vm](int64_t) {
        vm.pc = -1;
    };
    executeProgram(vm, program);

    std::cout << "First result is " << vm.rcv_value << std::endl;

    ParallelVm pvm;
    initializeParallelVm(pvm);
    std::cout << "Second result is " << executeParallel(pvm, program) << std::endl;

    return 0;
}
