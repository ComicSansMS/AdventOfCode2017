#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <spiralsquare.hpp>

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
}
