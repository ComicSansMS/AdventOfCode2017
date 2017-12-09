#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <register_vm.hpp>

TEST_CASE("Register VM")
{
    char const* input =
R"(b inc 5 if a > 1
a inc 1 if b < 5
c dec -10 if a >= 1
c inc -20 if c == 10
)";

    SECTION("Parse Comparison")
    {
        CHECK(parseComparison("==").value() == Comparison::Eq);
        CHECK(parseComparison("<").value() == Comparison::Lt);
        CHECK(parseComparison(">").value() == Comparison::Gt);
        CHECK(parseComparison("!=").value() == Comparison::Ne);
        CHECK(parseComparison("<=").value() == Comparison::Le);
        CHECK(parseComparison(">=").value() == Comparison::Ge);
        CHECK(!parseComparison("!!!!").has_value());
    }

    SECTION("Parse Guard")
    {
        auto g1 = parseGuard("if a > 1");
        CHECK(g1.register_name == "a");
        CHECK(g1.comparison == Comparison::Gt);
        CHECK(g1.argument == 1);

        auto g2 = parseGuard("if blub != -421");
        CHECK(g2.register_name == "blub");
        CHECK(g2.comparison == Comparison::Ne);
        CHECK(g2.argument == -421);
    }

    SECTION("Parse Operation")
    {
        CHECK(parseOperation("inc").value() == Operation::Increment);
        CHECK(parseOperation("dec").value() == Operation::Decrement);
        CHECK(!parseOperation("blub"));
    }

    SECTION("Parse Opcode")
    {
        auto op1 = parseOpcode("b inc 5 if a <= 1");
        CHECK(op1.register_name == "b");
        CHECK(op1.operation == Operation::Increment);
        CHECK(op1.argument == 5);
        CHECK(op1.guard.register_name == "a");
        CHECK(op1.guard.comparison == Comparison::Le);
        CHECK(op1.guard.argument == 1);

        auto op2 = parseOpcode("blub dec -455 if narf == -1231");
        CHECK(op2.register_name == "blub");
        CHECK(op2.operation == Operation::Decrement);
        CHECK(op2.argument == -455);
        CHECK(op2.guard.register_name == "narf");
        CHECK(op2.guard.comparison == Comparison::Eq);
        CHECK(op2.guard.argument == -1231);
    }

    SECTION("Parse Input")
    {
        auto vm = parseVM(input);
        REQUIRE(vm.instructions.size() == 4);
        CHECK(vm.instructions[0].register_name == "b");
        CHECK(vm.instructions[0].operation == Operation::Increment);
        CHECK(vm.instructions[0].argument == 5);
        CHECK(vm.instructions[0].guard.register_name == "a");
        CHECK(vm.instructions[0].guard.comparison == Comparison::Gt);
        CHECK(vm.instructions[0].guard.argument == 1);

        CHECK(vm.instructions[1].register_name == "a");
        CHECK(vm.instructions[1].operation == Operation::Increment);
        CHECK(vm.instructions[1].argument == 1);
        CHECK(vm.instructions[1].guard.register_name == "b");
        CHECK(vm.instructions[1].guard.comparison == Comparison::Lt);
        CHECK(vm.instructions[1].guard.argument == 5);

        CHECK(vm.instructions[2].register_name == "c");
        CHECK(vm.instructions[2].operation == Operation::Decrement);
        CHECK(vm.instructions[2].argument == -10);
        CHECK(vm.instructions[2].guard.register_name == "a");
        CHECK(vm.instructions[2].guard.comparison == Comparison::Ge);
        CHECK(vm.instructions[2].guard.argument == 1);

        CHECK(vm.instructions[3].register_name == "c");
        CHECK(vm.instructions[3].operation == Operation::Increment);
        CHECK(vm.instructions[3].argument == -20);
        CHECK(vm.instructions[3].guard.register_name == "c");
        CHECK(vm.instructions[3].guard.comparison == Comparison::Eq);
        CHECK(vm.instructions[3].guard.argument == 10);

        REQUIRE(vm.registers.size() == 3);
        CHECK(vm.registers.find("a") != end(vm.registers));
        CHECK(vm.registers["a"] == 0);
        CHECK(vm.registers.find("b") != end(vm.registers));
        CHECK(vm.registers["b"] == 0);
        CHECK(vm.registers.find("c") != end(vm.registers));
        CHECK(vm.registers["c"] == 0);
    }

    SECTION("Guard Evaluation")
    {
        Registers reg;
        auto g1 = parseGuard("if a > 1");
        reg["a"] = 1;
        CHECK(!evaluateGuard(g1, reg));
        reg["a"] = 10;
        CHECK(evaluateGuard(g1, reg));

        auto g2 = parseGuard("if blub != -421");
        reg["blub"] = 12;
        CHECK(evaluateGuard(g2, reg));
        reg["blub"] = -421;
        CHECK(!evaluateGuard(g2, reg));

        auto g3 = parseGuard("if a == 32");
        reg["a"] = 1;
        CHECK(!evaluateGuard(g3, reg));
        reg["a"] = 32;
        CHECK(evaluateGuard(g3, reg));

        auto g4 = parseGuard("if a < 182");
        reg["a"] = 182;
        CHECK(!evaluateGuard(g4, reg));
        reg["a"] = 181;
        CHECK(evaluateGuard(g4, reg));

        auto g5 = parseGuard("if a >= -12");
        reg["a"] = -13;
        CHECK(!evaluateGuard(g5, reg));
        reg["a"] = -12;
        CHECK(evaluateGuard(g5, reg));
        reg["a"] = -11;
        CHECK(evaluateGuard(g5, reg));

        auto g6 = parseGuard("if a <= 57");
        reg["a"] = 58;
        CHECK(!evaluateGuard(g6, reg));
        reg["a"] = 57;
        CHECK(evaluateGuard(g6, reg));
        reg["a"] = 56;
        CHECK(evaluateGuard(g6, reg));
    }

    SECTION("Opcode Execution")
    {
        auto op1 = parseOpcode("b inc 5 if a <= 1");
        Registers regs;
        regs["a"] = 3;
        regs["b"] = 32;
        executeOpcode(op1, regs);
        REQUIRE(regs.size() == 2);
        CHECK(regs["a"] == 3);
        CHECK(regs["b"] == 32);

        regs["a"] = -3;
        executeOpcode(op1, regs);
        REQUIRE(regs.size() == 2);
        CHECK(regs["a"] == -3);
        CHECK(regs["b"] == 37);

        auto op2 = parseOpcode("c dec -10 if a >= 1");
        regs["a"] = 12;
        regs["c"] = 10;
        executeOpcode(op2, regs);
        REQUIRE(regs.size() == 3);
        CHECK(regs["a"] == 12);
        CHECK(regs["b"] == 37);
        CHECK(regs["c"] == 20);
    }

    SECTION("Highest Value")
    {
        Registers regs;
        regs["foo"] = 1;
        regs["bar"] = -42;
        regs["blub"] = 25;
        regs["narf"] = 12;
        CHECK(highestRegisterValue(regs) == 25);
    }

    SECTION("Program Execution")
    {
        auto vm = parseVM(input);
        int const highest_during_execution = executeVM(vm);
        CHECK(highest_during_execution == 10);
        REQUIRE(vm.registers.size() == 3);
        CHECK(vm.registers["a"] == 1);
        REQUIRE(vm.registers.find("b") != end(vm.registers));
        CHECK(vm.registers["b"] == 0);
        CHECK(vm.registers["c"] == -10);
    }
}
