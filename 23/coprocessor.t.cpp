#include <catch.hpp>

#include <coprocessor.hpp>

#include <algorithm>
#include <numeric>

TEST_CASE("Coprocessor")
{
    char const* input = R"(set a 1
sub a 5
set b a
mul a b
jnz 0 -1
jnz a -12
)";

    SECTION("Parse input")
    {
        auto program = parseInput(input);

        REQUIRE(program.size() == 6);
        CHECK(std::get<Set>(program[0]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Set>(program[0]).value).n == 1);

        CHECK(std::get<Sub>(program[1]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Sub>(program[1]).value).n == 5);

        CHECK(std::get<Set>(program[2]).dest.r == 'b');
        CHECK(std::get<Register>(std::get<Set>(program[2]).value).r == 'a');

        CHECK(std::get<Mul>(program[3]).dest.r == 'a');
        CHECK(std::get<Register>(std::get<Mul>(program[3]).value).r == 'b');

        CHECK(std::get<Number>(std::get<Jnz>(program[4]).condition).n == 0);
        CHECK(std::get<Number>(std::get<Jnz>(program[4]).offset).n == -1);

        CHECK(std::get<Register>(std::get<Jnz>(program[5]).condition).r == 'a');
        CHECK(std::get<Number>(std::get<Jnz>(program[5]).offset).n == -12);
    }


    SECTION("VM Construction")
    {
        CoprocessorVm vm;

        CHECK(vm.mul_count == 0);
        CHECK(std::all_of(begin(vm.registers), end(vm.registers),
                          [](auto v) { return v == 0; }));
        CHECK(vm.pc == 0);
    }

    SECTION("Get Registers")
    {
        CoprocessorVm vm;
        std::iota(begin(vm.registers), end(vm.registers), 1);

        CHECK(getRegister(vm, Register{'a'}) == 1);
        CHECK(getRegister(vm, Register{'b'}) == 2);
        CHECK(getRegister(vm, Register{'c'}) == 3);
        CHECK(getRegister(vm, Register{'h'}) == 8);
    }

    SECTION("Get Value")
    {
        CoprocessorVm vm;
        std::iota(begin(vm.registers), end(vm.registers), 1);

        CHECK(getValue(vm, Register{'a'}) == 1);
        CHECK(getValue(vm, Register{'b'}) == 2);
        CHECK(getValue(vm, Register{'c'}) == 3);
        CHECK(getValue(vm, Register{'h'}) == 8);

        CHECK(getValue(vm, Number{ 42 }) == 42);
        CHECK(getValue(vm, Number{ 128 }) == 128);
    }

    SECTION("Stepwise Execution")
    {
        CoprocessorVm vm;
        auto const program = parseInput(input);

        executeInstruction(vm, program[0]);
        CHECK(vm.pc == 1);
        CHECK(getRegister(vm, Register{ 'a' }) == 1);

        executeInstruction(vm, program[1]);
        CHECK(vm.pc == 2);
        CHECK(getRegister(vm, Register{ 'a' }) == -4);

        executeInstruction(vm, program[2]);
        CHECK(vm.pc == 3);
        CHECK(getRegister(vm, Register{ 'b' }) == -4);

        CHECK(vm.mul_count == 0);
        executeInstruction(vm, program[3]);
        CHECK(vm.mul_count == 1);
        CHECK(vm.pc == 4);
        CHECK(getRegister(vm, Register{ 'a' }) == 16);

        executeInstruction(vm, program[4]);
        CHECK(vm.pc == 5);

        executeInstruction(vm, program[5]);
        CHECK(vm.pc == -7);
    }

    SECTION("Program execution")
    {
        CoprocessorVm vm;
        auto const program = parseInput(input);

        CHECK(executeProgram(vm, program) == 6);
        CHECK(vm.pc == -7);
        CHECK(vm.mul_count == 1);
        CHECK(getRegister(vm, Register{ 'a' }) == 16);
        CHECK(getRegister(vm, Register{ 'b' }) == -4);
        for(char r = 'c'; r <= 'h'; ++r) {
            CHECK(getRegister(vm, Register{ r }) == 0);
        }
    }

    SECTION("Prime sieve")
    {
        auto const s = sieve(100);
        CHECK(s[0] == 1);
        CHECK(s[1] == 1);
        CHECK(s[2] == 1);
        CHECK(s[3] == 1);
        CHECK(s[4] == 0);
        CHECK(s[5] == 1);
        CHECK(s[6] == 0);
        CHECK(s[7] == 1);
        CHECK(s[8] == 0);
        CHECK(s[9] == 0);
        CHECK(s[10] == 0);
        CHECK(s[11] == 1);
        CHECK(s[12] == 0);
        CHECK(s[13] == 1);
        CHECK(s[14] == 0);
        CHECK(s[15] == 0);
        CHECK(s[16] == 0);
        CHECK(s[17] == 1);
        CHECK(s[18] == 0);
        CHECK(s[19] == 1);
        CHECK(s[20] == 0);
        CHECK(s[21] == 0);
        CHECK(s[22] == 0);
        CHECK(s[23] == 1);
        CHECK(s[24] == 0);
        CHECK(s[25] == 0);

        CHECK(s[90] == 0);
        CHECK(s[91] == 0);
        CHECK(s[92] == 0);
        CHECK(s[93] == 0);
        CHECK(s[94] == 0);
        CHECK(s[95] == 0);
        CHECK(s[96] == 0);
        CHECK(s[97] == 1);
        CHECK(s[98] == 0);
        CHECK(s[99] == 0);
    }
}
