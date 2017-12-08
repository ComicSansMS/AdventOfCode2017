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
        CHECK(programs[0].weight == 66);
        CHECK(programs[0].dependencies.empty());

        CHECK(programs[12].name == "cntj");
        CHECK(programs[12].weight == 57);
        CHECK(programs[12].dependencies.empty());

        CHECK(programs[5].name == "fwft");
        CHECK(programs[5].weight == 72);
        REQUIRE(programs[5].dependencies.size() == 3);
        CHECK(programs[5].dependencies[0] == "ktlj");
        CHECK(programs[5].dependencies[1] == "cntj");
        CHECK(programs[5].dependencies[2] == "xhth");
    }

    SECTION("Determine Parents")
    {
        determineParents(programs);
        CHECK(programs[0].parent == "padx");
        CHECK(programs[0].dep_indices.empty());
        CHECK(programs[1].parent == "fwft");
        CHECK(programs[10].parent == "tknk");
        REQUIRE(programs[10].dep_indices.size() == 3);
        CHECK(programs[10].dep_indices == std::vector<int>{ 11, 2, 9 });
        int root = findRoot(programs);
        CHECK(programs[root].name == "tknk");
    }

    SECTION("Determine node weights")
    {
        determineParents(programs);
        calculateTotalWeights(programs);
        CHECK(programs[0].total_weight == 66);
        CHECK(programs[1].total_weight == 57);
        CHECK(programs[2].total_weight == 61);
        CHECK(programs[5].total_weight == 243);
        CHECK(programs[7].total_weight == 243);
        CHECK(programs[10].total_weight == 251);

        CHECK(programs[findRoot(programs)].total_weight == 778);
    }

    SECTION("Find weight mismatch")
    {
        determineParents(programs);
        calculateTotalWeights(programs);
        CHECK(findWeightMismatch(programs) == 60);
    }
}
