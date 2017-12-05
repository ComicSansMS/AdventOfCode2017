#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <spreadsheeet.hpp>

TEST_CASE("Spreadsheet")
{
    char const* input = "5 1 9 5\n7 5 3\n2 4 6 8";
    auto spread = parseInput(input);

    SECTION("Input Parsing")
    {
        REQUIRE(spread.numberOfRows() == 3);
        CHECK(spread.row(0) == std::vector<int>{5, 1, 9, 5});
        CHECK(spread.row(1) == std::vector<int>{7, 5, 3});
        CHECK(spread.row(2) == std::vector<int>{2, 4, 6, 8});
    }

    SECTION("Row Differences")
    {
        CHECK(calculateRowDifference(spread, 0) == 8);
        CHECK(calculateRowDifference(spread, 1) == 4);
        CHECK(calculateRowDifference(spread, 2) == 6);
    }

    SECTION("Checksum")
    {
        CHECK(calculateSpreadChecksum(spread) == 18);
    }
}
