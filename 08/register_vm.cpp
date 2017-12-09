#include <register_vm.hpp>

#include <cassert>
#include <regex>
#include <sstream>

std::optional<Comparison> parseComparison(std::string_view input)
{
    std::optional<Comparison> ret;
    if(input.compare("==") == 0) {
        ret = Comparison::Eq;
    } else if(input.compare("<") == 0) {
        ret = Comparison::Lt;
    } else if(input.compare(">") == 0) {
        ret = Comparison::Gt;
    } else if(input.compare("!=") == 0) {
        ret = Comparison::Ne;
    } else if(input.compare("<=") == 0) {
        ret = Comparison::Le;
    } else if(input.compare(">=") == 0) {
        ret = Comparison::Ge;
    }
    return ret;
}

Guard parseGuard(std::string_view input)
{
    std::regex guard_regex(R"(^if ([\w]+) ([<>!=]+) ([-]?[\d]+)$)");
    std::string str(input);
    std::match_results<std::string_view::iterator> matches;
    bool res = std::regex_match(input.begin(), input.end(), matches, guard_regex);
    assert(res);
    Guard ret;
    ret.register_name = matches[1];
    ret.comparison = parseComparison(std::string_view(&(*matches[2].first), matches[2].length())).value();
    ret.argument = std::stoi(matches[3]);
    return ret;
}

std::optional<Operation> parseOperation(std::string_view input)
{
    std::optional<Operation> ret;
    if(input.compare("inc") == 0) {
        ret = Operation::Increment;
    } else if(input.compare("dec") == 0) {
        ret = Operation::Decrement;
    }
    return ret;
}

Opcode parseOpcode(std::string_view input)
{
    std::regex opcode_regex(R"(^([\w]+) ([\w]+) ([-]?[\d]+) (if .*)$)");
    std::match_results<std::string_view::iterator> matches;
    bool res = std::regex_match(input.begin(), input.end(), matches, opcode_regex);
    assert(res);
    Opcode ret;
    ret.register_name = matches[1];
    ret.operation = parseOperation(std::string_view(&(*matches[2].first), matches[2].length())).value();
    ret.argument = std::stoi(matches[3]);
    ret.guard = parseGuard(std::string_view(&(*matches[4].first), matches[4].length()));
    return ret;
}

RegisterVM parseVM(std::string_view input)
{
    RegisterVM vm;
    std::stringstream sstr{std::string(input)};
    std::string line;
    while(std::getline(sstr, line)) {
        vm.instructions.push_back(parseOpcode(line));
        vm.registers[vm.instructions.back().register_name] = 0;
        vm.registers[vm.instructions.back().guard.register_name] = 0;
    }
    return vm;
}

bool evaluateGuard(Guard const& g, Registers const& reg)
{
    int const val = reg.find(g.register_name)->second;
    switch(g.comparison) {
    default:
    case Comparison::Eq: return val == g.argument;
    case Comparison::Lt: return val < g.argument;
    case Comparison::Gt: return val > g.argument;
    case Comparison::Ne: return val != g.argument;
    case Comparison::Le: return val <= g.argument;
    case Comparison::Ge: return val >= g.argument;
    }
}

void executeOpcode(Opcode const& op, Registers& reg)
{
    if(evaluateGuard(op.guard, reg)) {
        auto reg_it = reg.find(op.register_name);
        assert(reg_it != end(reg));
        switch(op.operation) {
        case Operation::Increment: reg_it->second += op.argument; break;
        case Operation::Decrement: reg_it->second -= op.argument; break;
        }
    }
}

int highestRegisterValue(Registers const& reg)
{
    return std::max_element(begin(reg), end(reg),
                            [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; })->second;
}

int executeVM(RegisterVM& vm)
{
    int max = 0;
    for(auto const& op : vm.instructions) {
        executeOpcode(op, vm.registers);
        max = std::max(max, highestRegisterValue(vm.registers));
    }
    return max;
}

