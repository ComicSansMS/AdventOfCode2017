#include <duet_vm.hpp>

#include <cassert>

#include <regex>
#include <string>

Program parseInput(std::string_view input)
{
    Program ret;

    std::regex rx_line(R"(^(\w\w\w) ([-\w\d]+)( ([-\w\d]+))?$)");

    auto parseValue = [](std::sub_match<std::string_view::iterator> match) -> Value {
        auto const input = match.str();
        if((input[0] >= 'a') && (input[0] <= 'z')) {
            return Register{ input[0] };
        } else {
            return Number{ std::stoi(input) };
        }
    };

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();
    for(auto it = it_begin; it != it_end; ++it) {
        auto match = *it;
        if(match[1].compare("snd") == 0) {
            ret.push_back(Snd{ parseValue(match[2]) });
        } else if(match[1].compare("set") == 0) {
            ret.push_back(Set{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("add") == 0) {
            ret.push_back(Add{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("mul") == 0) {
            ret.push_back(Mul{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("mod") == 0) {
            ret.push_back(Mod{ Register{ *match[2].first }, parseValue(match[4]) });
        } else if(match[1].compare("rcv") == 0) {
            ret.push_back(Rcv{ Register{ *match[2].first } });
        } else if(match[1].compare("jgz") == 0) {
            ret.push_back(Jgz{ parseValue(match[2]), parseValue(match[4]) });
        }
    }

    return ret;
}

int64_t getRegister(DuetVm const& vm, Register r)
{
    return vm.registers[r.r - 'a'];
}

int64_t getValue(DuetVm const& vm, Value v)
{
    struct Op {
        DuetVm vm;
        int64_t operator()(Number n) {
            return n.n;
        }
        int64_t operator()(Register r) {
            return getRegister(vm, r);
        }
    };
    return std::visit(Op{vm}, v);
}


int64_t executeInstruction(DuetVm& vm, Snd const& i)
{
    auto const val = getValue(vm, i.freq);
    vm.rcv_value = val;
    return val;
}

int64_t executeInstruction(DuetVm& vm, Set const& i)
{
    auto const val = getValue(vm, i.value);
    vm.registers[i.dest.r - 'a'] = val;
    return val;
}

int64_t executeInstruction(DuetVm& vm, Add const& i)
{
    auto& dest = vm.registers[i.dest.r - 'a'];
    auto const val = getValue(vm, i.value);
    dest += val;
    return dest;
}

int64_t executeInstruction(DuetVm& vm, Mul const& i)
{
    auto& dest = vm.registers[i.dest.r - 'a'];
    auto const val = getValue(vm, i.value);
    dest *= val;
    return dest;
}

int64_t executeInstruction(DuetVm& vm, Mod const& i)
{
    auto& dest = vm.registers[i.dest.r - 'a'];
    auto const val = getValue(vm, i.value);
    dest = dest % val;
    return dest;
}

int64_t executeInstruction(DuetVm& vm, Rcv const& i)
{
    if(getValue(vm, i.condition) != 0) {
        if(vm.rcv_callback) { vm.rcv_callback(vm.rcv_value); }
        return vm.rcv_value;
    }
    return 0;
}

int64_t executeInstruction(DuetVm& vm, Jgz const& i)
{
    if(getValue(vm, i.condition) != 0) {
        vm.pc += getValue(vm, i.offset) - 1;
        return vm.pc;
    }
    return 0;
}

int64_t executeInstruction(DuetVm& vm, Instruction const& i)
{
    return std::visit([&](auto instr) -> int64_t { ++vm.pc; return executeInstruction(vm, instr); }, i);
}

void executeProgram(DuetVm& vm, Program p)
{
    while((vm.pc >= 0) && (vm.pc < static_cast<int>(p.size()))) {
        executeInstruction(vm, p[vm.pc]);
    }
}

