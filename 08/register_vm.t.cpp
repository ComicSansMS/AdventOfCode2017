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
        auto op1 = parseOpcode("b inc 5 if a > 1");
        CHECK(op1.register_name == "b");
        CHECK(op1.operation == Operation::Increment);
        CHECK(op1.parameter == 5);
        CHECK(op1.guard.register_name == "a");
        CHECK(op1.guard.comparison == Comparison::Gt);
        CHECK(op1.guard.argument == 1);
    }

    SECTION("Parse Input")
    {
        auto vm = parseVM(input);
        REQUIRE(vm.instructions.size() == 4);
        CHECK(vm.instructions[0].register_name == "b");
        CHECK(vm.instructions[0].operation == Operation::Increment);
        CHECK(vm.instructions[0].parameter == 5);
        CHECK(vm.instructions[0].guard.register_name == "a");
        CHECK(vm.instructions[0].guard.comparison == Comparison::Gt);
        CHECK(vm.instructions[0].guard.argument == 1);

        CHECK(vm.instructions[1].register_name == "a");
        CHECK(vm.instructions[1].operation == Operation::Increment);
        CHECK(vm.instructions[1].parameter == 1);
        CHECK(vm.instructions[1].guard.register_name == "b");
        CHECK(vm.instructions[1].guard.comparison == Comparison::Lt);
        CHECK(vm.instructions[1].guard.argument == 5);

        CHECK(vm.instructions[2].register_name == "c");
        CHECK(vm.instructions[2].operation == Operation::Decrement);
        CHECK(vm.instructions[2].parameter == -10);
        CHECK(vm.instructions[2].guard.register_name == "a");
        CHECK(vm.instructions[2].guard.comparison == Comparison::Ge);
        CHECK(vm.instructions[2].guard.argument == 1);

        CHECK(vm.instructions[3].register_name == "c");
        CHECK(vm.instructions[3].operation == Operation::Increment);
        CHECK(vm.instructions[3].parameter == -20);
        CHECK(vm.instructions[3].guard.register_name == "c");
        CHECK(vm.instructions[3].guard.comparison == Comparison::Eq);
        CHECK(vm.instructions[3].guard.argument == 10);
    }
}
