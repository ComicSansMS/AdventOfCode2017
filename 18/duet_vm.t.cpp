#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <duet_vm.hpp>

#include <algorithm>
#include <numeric>

TEST_CASE("Duet, Part 1")
{
    char const* input = R"(set a 1
add a 2
mul a a
mod a 5
snd a
set a 0
rcv a
jgz a -1
set a 1
jgz a -2)";

    SECTION("Parse input")
    {
        auto program = parseInput(input);
        REQUIRE(program.size() == 10);

        CHECK(std::get<Set>(program[0]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Set>(program[0]).value).n == 1);

        CHECK(std::get<Add>(program[1]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Add>(program[1]).value).n == 2);

        CHECK(std::get<Mul>(program[2]).dest.r == 'a');
        CHECK(std::get<Register>(std::get<Mul>(program[2]).value).r == 'a');

        CHECK(std::get<Mod>(program[3]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Mod>(program[3]).value).n == 5);

        CHECK(std::get<Register>(std::get<Snd>(program[4]).value).r == 'a');

        CHECK(std::get<Set>(program[5]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Set>(program[5]).value).n == 0);

        CHECK(std::get<Rcv>(program[6]).reg.r == 'a');

        CHECK(std::get<Register>(std::get<Jgz>(program[7]).condition).r == 'a');
        CHECK(std::get<Number>(std::get<Jgz>(program[7]).offset).n == -1);

        CHECK(std::get<Set>(program[8]).dest.r == 'a');
        CHECK(std::get<Number>(std::get<Set>(program[8]).value).n == 1);

        CHECK(std::get<Register>(std::get<Jgz>(program[9]).condition).r == 'a');
        CHECK(std::get<Number>(std::get<Jgz>(program[9]).offset).n == -2);
    }

    SECTION("VM Construction")
    {
        DuetVm vm;

        CHECK(vm.rcv_value == 0);
        CHECK(std::all_of(begin(vm.registers), end(vm.registers),
                          [](auto v) { return v == 0; }));
        CHECK(vm.pc == 0);
    }

    SECTION("Get Registers")
    {
        DuetVm vm;
        std::iota(begin(vm.registers), end(vm.registers), 1);

        CHECK(getRegister(vm, Register{'a'}) == 1);
        CHECK(getRegister(vm, Register{'b'}) == 2);
        CHECK(getRegister(vm, Register{'c'}) == 3);
        CHECK(getRegister(vm, Register{'z'}) == 26);
    }

    SECTION("Get Value")
    {
        DuetVm vm;
        std::iota(begin(vm.registers), end(vm.registers), 1);

        CHECK(getValue(vm, Register{'a'}) == 1);
        CHECK(getValue(vm, Register{'b'}) == 2);
        CHECK(getValue(vm, Register{'c'}) == 3);
        CHECK(getValue(vm, Register{'z'}) == 26);

        CHECK(getValue(vm, Number{ 42 }) == 42);
        CHECK(getValue(vm, Number{ 128 }) == 128);
    }

    SECTION("Stepwise execution")
    {
        auto program = parseInput(input);
        DuetVm vm;
        bool callback_was_called = false;
        int64_t last_rcv = 0;
        vm.rcv_callback = [&callback_was_called, &last_rcv](int64_t rcv_freq) {
            callback_was_called = true;
            last_rcv = rcv_freq;
        };
    
        CHECK(vm.pc == 0);
        CHECK(executeInstruction(vm, program[0]) == 1);
        CHECK(vm.pc == 1);
        CHECK(vm.registers[0] == 1);

        CHECK(executeInstruction(vm, program[1]) == 3);
        CHECK(vm.pc == 2);
        CHECK(vm.registers[0] == 3);

        CHECK(executeInstruction(vm, program[2]) == 9);
        CHECK(vm.pc == 3);
        CHECK(vm.registers[0] == 9);

        CHECK(executeInstruction(vm, program[3]) == 4);
        CHECK(vm.pc == 4);
        CHECK(vm.registers[0] == 4);

        CHECK(vm.rcv_value == 0);
        CHECK(executeInstruction(vm, program[4]) == 4);
        CHECK(vm.pc == 5);
        CHECK(vm.registers[0] == 4);
        CHECK(vm.rcv_value == 4);

        CHECK(executeInstruction(vm, program[5]) == 0);
        CHECK(vm.pc == 6);
        CHECK(vm.registers[0] == 0);

        CHECK(!callback_was_called);
        CHECK(executeInstruction(vm, program[6]) == 0);
        CHECK(!callback_was_called);
        CHECK(vm.pc == 7);
        CHECK(vm.registers[0] == 0);

        CHECK(executeInstruction(vm, program[7]) == 0);
        CHECK(vm.pc == 8);
        CHECK(vm.registers[0] == 0);

        CHECK(executeInstruction(vm, program[8]) == 1);
        CHECK(vm.pc == 9);
        CHECK(vm.registers[0] == 1);

        CHECK(executeInstruction(vm, program[9]) == 7);
        CHECK(vm.pc == 7);
        CHECK(vm.registers[0] == 1);

        CHECK(executeInstruction(vm, program[7]) == 6);
        CHECK(vm.pc == 6);
        CHECK(vm.registers[0] == 1);

        CHECK(!callback_was_called);
        CHECK(executeInstruction(vm, program[6]) == 4);
        CHECK(callback_was_called);
        CHECK(last_rcv == 4);
        CHECK(vm.pc == 7);
        CHECK(vm.registers[0] == 1);

        // jgz should do nothing for negative values
        vm.registers[0] = -1;
        CHECK(executeInstruction(vm, program[7]) == 0);
        CHECK(vm.pc == 8);
        CHECK(vm.registers[0] == -1);
    }

    SECTION("Execution")
    {
        auto program = parseInput(input);
        DuetVm vm;

        vm.rcv_callback = [&vm](int64_t) {
            vm.pc = -1;
        };

        executeProgram(vm, program);
        CHECK(vm.rcv_value == 4);
    }
}

TEST_CASE("Duet, Part 2")
{
    char const* parallel_input = R"(snd 1
snd 2
snd p
rcv a
rcv b
rcv c
rcv d)";

    SECTION("Execute parallel")
    {
        auto parallel_program = parseInput(parallel_input);

        ParallelVm vm;
        initializeParallelVm(vm);

        CHECK(vm[0].queue.empty());
        CHECK(vm[1].queue.empty());
        CHECK(vm[0].rcv_value == 0);
        CHECK(vm[1].rcv_value == 0);

        CHECK(executeInstruction_Parallel(vm, 0, parallel_program[0]) == 1);
        CHECK(vm[0].rcv_value == 1);
        CHECK(vm[0].pc == 1);
        REQUIRE(vm[1].queue.size() == 1);
        CHECK(vm[1].queue[0] == 1);

        CHECK(executeInstruction_Parallel(vm, 0, parallel_program[1]) == 1);
        CHECK(vm[0].rcv_value == 2);
        CHECK(vm[0].pc == 2);
        CHECK(vm[1].queue == std::deque<int64_t>{ 1, 2 });

        CHECK(executeInstruction_Parallel(vm, 0, parallel_program[2]) == 1);
        CHECK(vm[0].rcv_value == 3);
        CHECK(vm[0].pc == 3);
        CHECK(vm[1].queue == std::deque<int64_t>{ 1, 2, 0 });

        CHECK(executeInstruction_Parallel(vm, 0, parallel_program[3]) == 0);
        CHECK(vm[0].pc == 3);


        CHECK(executeInstruction_Parallel(vm, 1, parallel_program[0]) == 1);
        CHECK(vm[1].rcv_value == 1);
        CHECK(vm[1].pc == 1);
        CHECK(vm[0].queue == std::deque<int64_t>{ 1 });

        CHECK(vm[0].registers[0] == 0);
        CHECK(executeInstruction_Parallel(vm, 0, parallel_program[3]) == 1);
        CHECK(vm[0].rcv_value == 3);
        CHECK(vm[0].pc == 4);
        CHECK(vm[0].queue.empty());
        CHECK(vm[0].registers[0] == 1);

        CHECK(executeInstruction_Parallel(vm, 1, parallel_program[1]) == 1);
        CHECK(vm[1].rcv_value == 2);
        CHECK(vm[1].pc == 2);
        CHECK(vm[0].queue == std::deque<int64_t>{ 2 });

        CHECK(executeInstruction_Parallel(vm, 1, parallel_program[2]) == 1);
        CHECK(vm[1].rcv_value == 3);
        CHECK(vm[1].pc == 3);
        CHECK(vm[0].queue == std::deque<int64_t>{ 2, 1 });
    }

    SECTION("Execute program parallel")
    {
        auto parallel_program = parseInput(parallel_input);

        ParallelVm vm;
        initializeParallelVm(vm);

        CHECK(executeParallel(vm, parallel_program) == 3);
    }
}
