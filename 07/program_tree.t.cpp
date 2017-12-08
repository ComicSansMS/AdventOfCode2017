#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <program_tree.hpp>

TEST_CASE("Program Tree")
{
    char const* input =
R"(pbga (66)
xhth (57)
ebii (61)
havc (66)
ktlj (57)
fwft (72) -> ktlj, cntj, xhth
qoyq (66)
padx (45) -> pbga, havc, qoyq
tknk (41) -> ugml, padx, fwft
jptl (61)
ugml (68) -> gyxo, ebii, jptl
gyxo (61)
cntj (57)
)";

    auto programs = parseInput(input);
    SECTION("Input Parsing")
    {
        REQUIRE(programs.size() == 13);
        CHECK(programs[0].name == "pbga");
        CHECK(programs[0].id == 66);
        CHECK(programs[0].dependencies.empty());

        CHECK(programs[12].name == "cntj");
        CHECK(programs[12].id == 57);
        CHECK(programs[12].dependencies.empty());

        CHECK(programs[5].name == "fwft");
        CHECK(programs[5].id == 72);
        REQUIRE(programs[5].dependencies.size() == 3);
        CHECK(programs[5].dependencies[0] == "ktlj");
        CHECK(programs[5].dependencies[1] == "cntj");
        CHECK(programs[5].dependencies[2] == "xhth");
    }

    SECTION("Determine Parents")
    {
        determineParents(programs);
        CHECK(programs[0].parent == "padx");
        CHECK(programs[1].parent == "fwft");
        CHECK(programs[10].parent == "tknk");
        int root = findRoot(programs);
        CHECK(programs[root].name == "tknk");
    }
}
