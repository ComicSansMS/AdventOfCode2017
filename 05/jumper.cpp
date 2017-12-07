#include <jumper.hpp>

#include <sstream>
#include <string>

std::vector<int> parseInput(std::string_view input)
{
    std::stringstream sstr{std::string(input)};

    std::vector<int> ret;
    std::string line;
    while(std::getline(sstr, line)) {
        ret.push_back(std::stoi(line));
    }

    return ret;
}

int runJumperVM(std::vector<int>& tape)
{
    int pc = 0;
    int steps = 0;
    while((pc >= 0) && (pc < tape.size())) {
        int const jump = tape[pc];
        ++tape[pc];
        pc += jump;
        ++steps;
    }
    return steps;
}

int runStrangeJumperVM(std::vector<int>& tape)
{
    int pc = 0;
    int steps = 0;
    while((pc >= 0) && (pc < tape.size())) {
        int const jump = tape[pc];
        if(jump >= 3) { --tape[pc]; } else { ++tape[pc]; }
        pc += jump;
        ++steps;
    }
    return steps;
}

