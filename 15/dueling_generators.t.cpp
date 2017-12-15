#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <dueling_generators.hpp>

TEST_CASE("Dueling generators")
{
    char const* input = R"(Generator A starts with 65
Generator B starts with 8921)";

    SECTION("Parse input")
    {
        auto generators = parseInput(input);
        REQUIRE(generators.size() == 2);
        CHECK(generators[0].factor == 16807);
        CHECK(generators[0].current_value == 65);
        CHECK(generators[1].factor == 48271);
        CHECK(generators[1].current_value == 8921);
    }

    SECTION("Generate")
    {
        auto g = parseInput(input);
        CHECK(g[0].current_value == 65);
        CHECK(g[1].current_value == 8921);

        generate(g);
        CHECK(g[0].current_value == 1092455);
        CHECK(g[1].current_value == 430625591);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g[0].current_value == 1181022009);
        CHECK(g[1].current_value == 1233683848);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g[0].current_value == 245556042);
        CHECK(g[1].current_value == 1431495498);
        CHECK(does_match(g));
        generate(g);
        CHECK(g[0].current_value == 1744312007);
        CHECK(g[1].current_value == 137874439);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g[0].current_value == 1352636452);
        CHECK(g[1].current_value == 285222916);
        CHECK(!does_match(g));
    }
}
