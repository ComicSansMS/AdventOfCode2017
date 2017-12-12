#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <pipe_network.hpp>

#include <sstream>
#include <string>

TEST_CASE("Pipe Network")
{
    char const* input = R"(0 <-> 2
1 <-> 1
2 <-> 0, 3, 4
3 <-> 2, 4
4 <-> 2, 3, 6
5 <-> 6
6 <-> 4, 5)";

    SECTION("Parse single line")
    {
        auto c1 = parseLine("0 <-> 2");
        CHECK(c1.node == 0);
        REQUIRE(c1.connected_nodes.size() == 1);
        CHECK(c1.connected_nodes[0] == 2);

        auto c2 = parseLine("22 <-> 0, 135, 44");
        CHECK(c2.node == 22);
        REQUIRE(c2.connected_nodes.size() == 3);
        CHECK(c2.connected_nodes[0] == 0);
        CHECK(c2.connected_nodes[1] == 135);
        CHECK(c2.connected_nodes[2] == 44);
    }

    SECTION("Parse input")
    {
        auto cn = parseInput(input);
        REQUIRE(cn.size() == 7);
        for(int i=0; i<cn.size(); ++i) {
            CHECK(cn[i].node == i);
        }
        CHECK(cn[4].connected_nodes == std::vector<int>{ 2, 3, 6 });
    }

    SECTION("Build graph")
    {
        auto g = buildGraph(parseInput(input));
        CHECK(g.edges[0].size() == 1);
        CHECK(g.edges[0].find(2) != end(g.edges[0]));
        CHECK(g.edges[6].size() == 2);
        CHECK(g.edges[6].find(4) != end(g.edges[6]));
        CHECK(g.edges[6].find(5) != end(g.edges[6]));
    }

    SECTION("Connected group")
    {
        auto g = buildGraph(parseInput(input));
        CHECK(getConnectedGroup(g, 0) == std::vector<int>{ 0, 2, 3, 4, 5, 6 });
    }

    SECTION("All connected groups")
    {
        auto g = buildGraph(parseInput(input));
        CHECK(getAllConnectedGroups(g).size() == 2);
    }
}
