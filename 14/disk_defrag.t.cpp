#include <catch.hpp>

#include <disk_defrag.hpp>

TEST_CASE("Disk Defrag")
{
    char const* input = "flqrgnkx";

    SECTION("Bitset from Hash")
    {
        char const* hash = "a0c20170000000000000000000000000";
        std::bitset<128> bitset;

        setBitsetFromHash(bitset, hash);

        std::bitset<32> expected(0b10100000110000100000000101110000);
        for(std::size_t i=0; i<32; ++i) {
            CHECK(expected[31 - i] == bitset[i]);
        }
        for(std::size_t i=32; i<128; ++i) {
            CHECK(!bitset[i]);
        }
    }

    SECTION("Grid generation")
    {
        Grid g(input);

        //##.#.#..
        CHECK(g.get(0,0));
        CHECK(g.get(0,1));
        CHECK(!g.get(0,2));
        CHECK(g.get(0,3));
        CHECK(!g.get(0,4));
        CHECK(g.get(0,5));
        CHECK(!g.get(0,6));
        CHECK(!g.get(0,7));
        //.#.#.#.#
        CHECK(!g.get(1,0));
        CHECK(g.get(1,1));
        CHECK(!g.get(1,2));
        CHECK(g.get(1,3));
        CHECK(!g.get(1,4));
        CHECK(g.get(1,5));
        CHECK(!g.get(1,6));
        CHECK(g.get(1,7));
        //....#.#.
        CHECK(!g.get(2,0));
        CHECK(!g.get(2,1));
        CHECK(!g.get(2,2));
        CHECK(!g.get(2,3));
        CHECK(g.get(2,4));
        CHECK(!g.get(2,5));
        CHECK(g.get(2,6));
        CHECK(!g.get(2,7));
        //#.#.##.#
        CHECK(g.get(3,0));
        CHECK(!g.get(3,1));
        CHECK(g.get(3,2));
        CHECK(!g.get(3,3));
        CHECK(g.get(3,4));
        CHECK(g.get(3,5));
        CHECK(!g.get(3,6));
        CHECK(g.get(3,7));
        //.##.#...
        CHECK(!g.get(4,0));
        CHECK(g.get(4,1));
        CHECK(g.get(4,2));
        CHECK(!g.get(4,3));
        CHECK(g.get(4,4));
        CHECK(!g.get(4,5));
        CHECK(!g.get(4,6));
        CHECK(!g.get(4,7));
        //##..#..#
        CHECK(g.get(5,0));
        CHECK(g.get(5,1));
        CHECK(!g.get(5,2));
        CHECK(!g.get(5,3));
        CHECK(g.get(5,4));
        CHECK(!g.get(5,5));
        CHECK(!g.get(5,6));
        CHECK(g.get(5,7));
        //.#...#..
        CHECK(!g.get(6,0));
        CHECK(g.get(6,1));
        CHECK(!g.get(6,2));
        CHECK(!g.get(6,3));
        CHECK(!g.get(6,4));
        CHECK(g.get(6,5));
        CHECK(!g.get(6,6));
        CHECK(!g.get(6,7));
        //##.#.##.
        CHECK(g.get(7,0));
        CHECK(g.get(7,1));
        CHECK(!g.get(7,2));
        CHECK(g.get(7,3));
        CHECK(!g.get(7,4));
        CHECK(g.get(7,5));
        CHECK(g.get(7,6));
        CHECK(!g.get(7,7));

        CHECK(g.count() == 8108);
    }

    SECTION("Test Flood Fill")
    {
        Grid g(input);
        auto gd = g.getGridData();

        // region
        CHECK(gd[0][0]);
        CHECK(gd[0][1]);
        CHECK(gd[1][1]);

        // no man's land
        CHECK(!gd[1][0]);
        CHECK(!gd[2][0]);
        CHECK(!gd[2][1]);
        CHECK(!gd[2][2]);
        CHECK(!gd[1][2]);
        CHECK(!gd[0][2]);

        // neighboring region
        CHECK(gd[0][3]);
        CHECK(gd[1][3]);

        floodFill(gd, 0, 0);

        // region is cleared
        CHECK(!gd[0][0]);
        CHECK(!gd[0][1]);
        CHECK(!gd[1][1]);

        // no man's land still intact
        CHECK(!gd[1][0]);
        CHECK(!gd[2][0]);
        CHECK(!gd[2][1]);
        CHECK(!gd[2][2]);
        CHECK(!gd[1][2]);
        CHECK(!gd[0][2]);

        // neighboring region still intact
        CHECK(gd[0][3]);
        CHECK(gd[1][3]);
    }

    SECTION("Count regions")
    {
        Grid g(input);
        CHECK(g.countRegions() == 1242);
    }
}
