#include <catch.hpp>

#include <spiralsquare.hpp>

#include <iostream>

TEST_CASE("Spiralsquare")
{

    SECTION("Determine Ring")
    {
        CHECK(getRing(1) == 1);
        CHECK(getRing(8) == 3);
        CHECK(getRing(11) == 5);
        CHECK(getRing(18) == 5);
    }

    SECTION("Check distance")
    {
        CHECK(getDistance(1) == 0);
        CHECK(getDistance(8) == 1);
        CHECK(getDistance(11) == 2);
        CHECK(getDistance(18) == 3);
        CHECK(getDistance(12) == 3);
        CHECK(getDistance(23) == 2);
        CHECK(getDistance(1024) == 31);
    }

    SECTION("Check distance on generated field")
    {
        Field f(getRing(1024));
        f.fillField();
        CHECK(f.getDistanceFromCenter(1) == 0);
        CHECK(f.getDistanceFromCenter(8) == 1);
        CHECK(f.getDistanceFromCenter(11) == 2);
        CHECK(f.getDistanceFromCenter(18) == 3);
        CHECK(f.getDistanceFromCenter(12) == 3);
        CHECK(f.getDistanceFromCenter(23) == 2);
        CHECK(f.getDistanceFromCenter(1024) == 31);
    }

    SECTION("Complicated filling")
    {
        Field f(5);
        int searched = f.fillComplicated(55);
        f.printField(std::cout);
        CHECK(f.getCell(0, 0) == 1);
        CHECK(f.getCell(0, 2) == 806);
        CHECK(f.getCell(-2, 2) == 362);
        CHECK(f.getCell(0, -2) == 133);
        CHECK(searched == 57);
    }
}
