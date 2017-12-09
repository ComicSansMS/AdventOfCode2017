#ifndef ADVENT_OF_CODE_08_REGISTER_VM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_REGISTER_VM_HPP_INCLUDE_GUARD

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class Operation {
    Increment,
    Decrement
};

enum class Comparison {
    Eq, // ==
    Lt, // <
    Gt, // >
    Ne, // !=
    Le, // <=
    Ge, // >=
};

struct Guard {
    std::string register_name;
    Comparison comparison;
    int argument;
};

struct Opcode {
    std::string register_name;
    Operation operation;
    int parameter;
    Guard guard;
};

using Tape = std::vector<Opcode>;
struct RegisterVM {
    Tape instructions;
    std::unordered_map<std::string, int> registers;
};

std::optional<Comparison> parseComparison(std::string_view input);

Guard parseGuard(std::string_view input);

std::optional<Operation> parseOperation(std::string_view input);

Opcode parseOpcode(std::string_view input);

RegisterVM parseVM(std::string_view input);

#endif
