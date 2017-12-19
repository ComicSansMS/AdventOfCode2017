#include <duet_vm.hpp>

#include <cassert>
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
        DuetVm const& vm;
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
    auto const val = getValue(vm, i.value);
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
    if(getValue(vm, i.reg) != 0) {
        if(vm.rcv_callback) { vm.rcv_callback(vm.rcv_value); }
        return vm.rcv_value;
    }
    return 0;
}

int64_t executeInstruction(DuetVm& vm, Jgz const& i)
{
    if(getValue(vm, i.condition) > 0) {
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
    while((vm.pc >= 0) && (vm.pc < static_cast<int64_t>(p.size()))) {
        executeInstruction(vm, p[vm.pc]);
    }
}

namespace {
struct ParallelOp {
    ParallelVm& vm;
    int active_vm;
    template<typename T>
    int64_t operator()(T const& op) {
        executeInstruction(vm[active_vm], op);
        return 1;
    }

    int64_t operator()(Snd const& snd) {
        auto const val = getValue(vm[active_vm], snd.value);
        vm[1 - active_vm].queue.push_back(val);
        ++vm[active_vm].rcv_value;
        return 1;
    }

    int64_t operator()(Rcv const& rcv) {
        if(vm[active_vm].queue.empty()) {
            return 0;
        }
        vm[active_vm].registers[rcv.reg.r - 'a'] = vm[active_vm].queue.front();
        vm[active_vm].queue.pop_front();
        return 1;
    }
};
}

void initializeParallelVm(ParallelVm& vm)
{
    vm[0].registers['p' - 'a'] = 0;
    vm[1].registers['p' - 'a'] = 1;
}

int64_t executeInstruction_Parallel(ParallelVm& vm, int active_vm, Instruction i)
{
    ParallelOp op{ vm, active_vm };
    int64_t const ret = std::visit(op, i);
    if(ret != 0) { ++vm[active_vm].pc; }
    return ret;
}

int64_t executeParallel(ParallelVm& vm, Program p)
{
    int active_vm = 0;
    int n_halted = 0;
    for(;;)
    {
        while((vm[active_vm].pc >= 0) && (vm[active_vm].pc < static_cast<int64_t>(p.size())) &&
              (executeInstruction_Parallel(vm, active_vm, p[vm[active_vm].pc]) != 0)) { n_halted = 0; }
        ++n_halted;
        if(n_halted == 2) { break; }
        active_vm = 1 - active_vm;
    }
    return vm[1].rcv_value;
}
