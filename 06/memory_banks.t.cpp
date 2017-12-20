#include <catch.hpp>

#include <memory_banks.hpp>

TEST_CASE("Memory Banks")
{
    char const* input = "0\t2\t7\t0";
    MemoryArray banks = parseInput(input);

    SECTION("Input Parsing")
    {
        REQUIRE(banks.size() == 4);
        CHECK(banks[0] == 0);
        CHECK(banks[1] == 2);
        CHECK(banks[2] == 7);
        CHECK(banks[3] == 0);
    }

    SECTION("Banks to String")
    {
        CHECK(banksToString(banks) == "0-2-7-0");
    }

    SECTION("Iterate")
    {
        iterate(banks);
        CHECK(banks == MemoryArray{ 2, 4, 1, 2 });
        iterate(banks);
        CHECK(banks == MemoryArray{ 3, 1, 2, 3 });
        iterate(banks);
        CHECK(banks == MemoryArray{ 0, 2, 3, 4 });
        iterate(banks);
        CHECK(banks == MemoryArray{ 1, 3, 4, 1 });
        iterate(banks);
        CHECK(banks == MemoryArray{ 2, 4, 1, 2 });
    }

    SECTION("Redistribute")
    {
        CHECK(redistribute(banks) == 5);
        CHECK(redistribute(banks) == 4);
    }
}
