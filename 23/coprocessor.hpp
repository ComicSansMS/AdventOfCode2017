#ifndef ADVENT_OF_CODE_23_COPROCESSOR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_23_COPROCESSOR_HPP_INCLUDE_GUARD

#include <array>
#include <cstdint>
#include <string_view>
#include <variant>
#include <vector>

struct Register {
    char r;
};

struct Number {
    int64_t n;
};

using Value = std::variant<Register, Number>;

struct Set {
    Register dest;
    Value value;
};

struct Sub {
    Register dest;
    Value value;
};

struct Mul {
    Register dest;
    Value value;
};

struct Jnz {
    Value condition;
    Value offset;
};

using Instruction = std::variant<Set, Sub, Mul, Jnz>;

using Program = std::vector<Instruction>;

Program parseInput(std::string_view input);

struct CoprocessorVm {
    std::array<int64_t, 8> registers = {};
    std::int64_t pc = 0;
    std::int64_t mul_count = 0;
};

int64_t getRegister(CoprocessorVm const& vm, Register r);

int64_t getValue(CoprocessorVm const& vm, Value v);

int64_t executeInstruction(CoprocessorVm& vm, Set const& i);
int64_t executeInstruction(CoprocessorVm& vm, Sub const& i);
int64_t executeInstruction(CoprocessorVm& vm, Mul const& i);
int64_t executeInstruction(CoprocessorVm& vm, Jnz const& i);

int64_t executeInstruction(CoprocessorVm& vm, Instruction const& i);

int64_t executeProgram(CoprocessorVm& vm, Program const& program);

std::vector<char> sieve(int limit);

#endif
