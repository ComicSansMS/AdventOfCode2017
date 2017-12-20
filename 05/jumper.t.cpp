#include <catch.hpp>

#include <jumper.hpp>

TEST_CASE("Jumper")
{
    char const* input = "0\n3\n0\n1\n-3";
    SECTION("Input Parsing")
    {
        auto iseq = parseInput(input);
        REQUIRE(iseq.size() == 5);
        CHECK(iseq[0] == 0);
        CHECK(iseq[1] == 3);
        CHECK(iseq[2] == 0);
        CHECK(iseq[3] == 1);
        CHECK(iseq[4] == -3);
    }

    SECTION("Jumper VM")
    {
        auto tape = parseInput(input);
        CHECK(runJumperVM(tape) == 5);
        CHECK(tape == std::vector<int>{ 2, 5, 0, 1, -2 });
    }

    SECTION("Strange Jumper VM")
    {
        auto tape = parseInput(input);
        CHECK(runStrangeJumperVM(tape) == 10);
        CHECK(tape == std::vector<int>{ 2, 3, 2, 3, -1 });
    }
}
