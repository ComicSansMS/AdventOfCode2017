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

TEST_CASE("Spreadsheet Even Divide")
{
    char const* input = "5 9 2 8\n9 4 7 3\n3 8 6 5";
    auto spread = parseInput(input);
    SECTION("Row Even Divide")
    {
        CHECK(calculateRowEvenDivide(spread, 0) == 4);
        CHECK(calculateRowEvenDivide(spread, 1) == 3);
        CHECK(calculateRowEvenDivide(spread, 2) == 2);
    }

    SECTION("Checksum Even Divide")
    {
        CHECK(calculateSpreadChecksumEvenDivide(spread) == 9);
    }
}
