#include <catch.hpp>

#include <bridge_builder.hpp>

TEST_CASE("Bridge Builder")
{
    char const* input = R"(0/2
2/2
2/3
3/4
3/5
0/1
10/1
9/10
)";

    SECTION("Parse input")
    {
        auto const ports = parseInput(input);

        REQUIRE(ports.size() == 8);
        CHECK(ports[0][0] == 0);
        CHECK(ports[0][1] == 2);
        CHECK(ports[1][0] == 2);
        CHECK(ports[1][1] == 2);
        CHECK(ports[2][0] == 2);
        CHECK(ports[2][1] == 3);
        CHECK(ports[3][0] == 3);
        CHECK(ports[3][1] == 4);
        CHECK(ports[4][0] == 3);
        CHECK(ports[4][1] == 5);
        CHECK(ports[5][0] == 0);
        CHECK(ports[5][1] == 1);
        CHECK(ports[6][0] == 10);
        CHECK(ports[6][1] == 1);
        CHECK(ports[7][0] == 9);
        CHECK(ports[7][1] == 10);
    }

    SECTION("Enumerate possibilities")
    {
        auto const ports = parseInput(input);

        auto const bridges = getBridges(ports);
        REQUIRE(bridges.size() == 11);
        std::vector<Ports> expected_bridges;
        expected_bridges.push_back(Ports{ Port{0,1} });
        expected_bridges.push_back(Ports{ Port{0,1}, Port{10,1} });
        expected_bridges.push_back(Ports{ Port{0,1}, Port{10,1}, Port{9,10} });
        expected_bridges.push_back(Ports{ Port{0,2} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,3} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,3}, Port{3,4} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,3}, Port{3,5} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,2} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,2}, Port{2,3} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,2}, Port{2,3}, Port{3,4} });
        expected_bridges.push_back(Ports{ Port{0,2}, Port{2,2}, Port{2,3}, Port{3,5} });

        for(auto const& b : expected_bridges) {
            CHECK(std::find(begin(bridges), end(bridges), b) != end(bridges));
        }
    }

    SECTION("Bridge strength")
    {
        CHECK(getBridgeStrength(Ports{ }) == 0);
        CHECK(getBridgeStrength(Ports{ Port{0,1} }) == 1);
        CHECK(getBridgeStrength(Ports{ Port{0,1}, Port{10,1}, Port{9,10} }) == 31);
        CHECK(getBridgeStrength(Ports{ Port{0,2}, Port{2,2}, Port{2,3}, Port{3,5} }) == 19);
    }

    SECTION("Strongest bridge")
    {
        auto const ports = parseInput(input);
        auto const bridges = getBridges(ports);

        CHECK(getStrongestStrength(bridges) == 31);
    }

    SECTION("Longest strongest bridge")
    {
        auto const ports = parseInput(input);
        auto const bridges = getBridges(ports);

        CHECK(getLongestStrongestStrength(bridges) == 19);
    }
}
