#ifndef ADVENT_OF_CODE_18_DUET_VM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_18_DUET_VM_HPP_INCLUDE_GUARD

#include <array>
#include <cstdint>
#include <functional>
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

struct Snd {
    Value freq;
};

struct Set {
    Register dest;
    Value value;
};

struct Add {
    Register dest;
    Value value;
};

struct Mul {
    Register dest;
    Value value;
};

struct Mod {
    Register dest;
    Value value;
};

struct Rcv {
    Register condition;
};

struct Jgz {
    Value condition;
    Value offset;
};

using Instruction = std::variant<Snd, Set, Add, Mul, Mod, Rcv, Jgz>;

using Program = std::vector<Instruction>;

Program parseInput(std::string_view input);

struct DuetVm {
    std::array<int64_t, 26> registers = {};
    int64_t rcv_value = 0;
    int64_t pc = 0;
    std::function<void(int64_t)> rcv_callback;
};

int64_t getRegister(DuetVm const& vm, Register r);
int64_t getValue(DuetVm const& vm, Value v);

int64_t executeInstruction(DuetVm& vm, Snd const& i);
int64_t executeInstruction(DuetVm& vm, Set const& i);
int64_t executeInstruction(DuetVm& vm, Add const& i);
int64_t executeInstruction(DuetVm& vm, Mul const& i);
int64_t executeInstruction(DuetVm& vm, Mod const& i);
int64_t executeInstruction(DuetVm& vm, Rcv const& i);
int64_t executeInstruction(DuetVm& vm, Jgz const& i);

int64_t executeInstruction(DuetVm& vm, Instruction const& i);

void executeProgram(DuetVm& vm, Program p);

#endif
