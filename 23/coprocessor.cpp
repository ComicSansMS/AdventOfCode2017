#include <coprocessor.hpp>

#include <cassert>
#include <cmath>
#include <regex>
#include <sstream>
#include <string>

Program parseInput(std::string_view input)
{
    Program ret;

    std::regex rx_line(R"(^(\w\w\w) ([-\w\d]+)( ([-\w\d]+))?$)");

    auto parseValue = [](std::sub_match<std::string::const_iterator> match) -> Value {
        auto const input = match.str();
        if((input[0] >= 'a') && (input[0] <= 'z')) {
            return Register{ input[0] };
        } else {
            return Number{ std::stoi(input) };
        }
    };

    std::string line;
    std::stringstream sstr{std::string(input)};
    while(std::getline(sstr, line)) {
        std::smatch match;
        std::regex_match(line, match, rx_line);
        if(match[1].compare("set") == 0) {
            ret.push_back(Set{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("sub") == 0) {
            ret.push_back(Sub{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("mul") == 0) {
            ret.push_back(Mul{ Register{ *match[2].first }, parseValue(match[4]) });
        } else {
            assert(match[1].compare("jnz") == 0);
            ret.push_back(Jnz{ parseValue(match[2]), parseValue(match[4]) });
        }
    }

    return ret;
}

int64_t getRegister(CoprocessorVm const& vm, Register r)
{
    return vm.registers[r.r - 'a'];
}

int64_t getValue(CoprocessorVm const& vm, Value v)
{
    struct Op {
        CoprocessorVm const& vm;
        int64_t operator()(Number n) {
            return n.n;
        }
        int64_t operator()(Register r) {
            return getRegister(vm, r);
        }
    };
    return std::visit(Op{vm}, v);
}

int64_t executeInstruction(CoprocessorVm& vm, Set const& i)
{
    auto const v = getValue(vm, i.value);
    vm.registers[i.dest.r - 'a'] = v;
    return v;
}

int64_t executeInstruction(CoprocessorVm& vm, Sub const& i)
{
    auto const v = getValue(vm, i.value);
    vm.registers[i.dest.r - 'a'] -= v;
    return vm.registers[i.dest.r - 'a'];
}

int64_t executeInstruction(CoprocessorVm& vm, Mul const& i)
{
    auto const v = getValue(vm, i.value);
    vm.registers[i.dest.r - 'a'] *= v;
    ++vm.mul_count;
    return vm.registers[i.dest.r - 'a'];
}

int64_t executeInstruction(CoprocessorVm& vm, Jnz const& i)
{
    auto const v = getValue(vm, i.condition);
    if(v != 0) { vm.pc += (getValue(vm, i.offset) - 1); }
    return vm.pc + 1;
}

int64_t executeInstruction(CoprocessorVm& vm, Instruction const& i)
{
    return std::visit([&vm](auto const& i) { ++vm.pc; return executeInstruction(vm, i); }, i);
}

int64_t executeProgram(CoprocessorVm& vm, Program const& program)
{
    int64_t num_cycles = 0;
    while((vm.pc >= 0) && (vm.pc < static_cast<int>(program.size()))) {
        executeInstruction(vm, program[vm.pc]);
        ++num_cycles;
    }
    return num_cycles;
}

std::vector<char> sieve(int limit)
{
    std::vector<char> s(limit, 1);
    for(int i=2; i<=static_cast<int>(std::sqrt(limit)); ++i) {
        if(s[i]) {
            for(int j=2*i; j<limit; j+=i) {
                s[j] = 0;
            }
        }
    }
    return s;
}
