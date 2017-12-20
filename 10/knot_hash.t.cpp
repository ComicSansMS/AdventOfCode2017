#include <catch.hpp>

#include <knot_hash.hpp>

TEST_CASE("Knot Hash")
{
    SECTION("Parse input")
    {
        CHECK(parseLengths("212,254,178,237") == std::vector<int>{212,254,178,237});
    }

    SECTION("Parse input ASCII")
    {
        CHECK(parseLengths_ASCII("1,2,3") == std::vector<int>{49,44,50,44,51,17,31,73,47,23});
    }

    SECTION("Build initial String Circle")
    {
        auto sc = buildInitialCircle();
        REQUIRE(sc.size() == 256);
        for(int i=0; i<256; ++i) {
            CHECK(sc[i] == i);
        }
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

    SECTION("Knot Hash Single Round")
    {
        StringCircle sc{ 0, 1, 2, 3, 4 };
        std::vector<int> lengths{ 3, 4, 1, 5 };
        KnotHashState state{ 0, 0 };
        knotHashSingleRound(sc, lengths, state);
        CHECK(sc == StringCircle{ 3, 4, 2, 1, 0 });
    }

    SECTION("Densify")
    {
        StringCircle sc{65, 27, 9, 1, 4, 3, 40, 50, 91, 7, 6, 0, 2, 5, 68, 22};
        CHECK(getDenseHash(sc) == std::vector<int>(1, 64));
    }

    SECTION("Hexify")
    {
        CHECK(hexify(std::vector<int>{ 64, 7, 255 }) == "4007ff");
    }

    SECTION("Knot Hash")
    {
        CHECK(knotHash("") == "a2582a3a0e66e6e86e3812dcb672a272");
        CHECK(knotHash("AoC 2017") == "33efeb34ea91902bb2f59c9920caa6cd");
        CHECK(knotHash("1,2,3") == "3efbe78a8d82f29979031a4aa0b16a9d");
        CHECK(knotHash("1,2,4") == "63960835bcdc130f0b66d7ff4f6a5a8e");
    }
}
