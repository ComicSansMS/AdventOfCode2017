#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <knot_hash.hpp>

TEST_CASE("Knot Hash")
{
    SECTION("Parse input")
    {
        CHECK(parseLengths("212,254,178,237") == std::vector<int>{212,254,178,237});
    }

    SECTION("Circular Reverse")
    {
        StringCircle sc{1,2,3,4,5};

        circularReverse(sc, 0, 5);
        CHECK(sc == StringCircle{5, 4, 3, 2, 1});

        circularReverse(sc, 0, 3);
        CHECK(sc == StringCircle{3, 4, 5, 2, 1});

        circularReverse(sc, 3, 2);
        CHECK(sc == StringCircle{3, 4, 5, 1, 2});

        circularReverse(sc, 3, 5);
        CHECK(sc == StringCircle{3, 2, 1, 5, 4});
    }

    SECTION("Knot Hash")
    {
        StringCircle sc{ 0, 1, 2, 3, 4 };
        std::vector<int> lengths{ 3, 4, 1, 5 };
        knotHash(sc, lengths);
        CHECK(sc == StringCircle{ 3, 4, 2, 1, 0 });
    }
}
