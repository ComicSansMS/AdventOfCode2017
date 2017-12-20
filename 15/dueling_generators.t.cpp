#include <catch.hpp>

#include <dueling_generators.hpp>

TEST_CASE("Dueling generators")
{
    char const* input = R"(Generator A starts with 65
Generator B starts with 8921)";

    SECTION("Parse input")
    {
        auto generators = parseInput(input);
        CHECK(generators.a.factor == 16807);
        CHECK(generators.a.current_value == 65);
        CHECK(generators.b.factor == 48271);
        CHECK(generators.b.current_value == 8921);
    }

    SECTION("Generate")
    {
        auto g = parseInput(input);
        CHECK(g.a.current_value == 65);
        CHECK(g.b.current_value == 8921);

        generate(g);
        CHECK(g.a.current_value == 1092455);
        CHECK(g.b.current_value == 430625591);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g.a.current_value == 1181022009);
        CHECK(g.b.current_value == 1233683848);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g.a.current_value == 245556042);
        CHECK(g.b.current_value == 1431495498);
        CHECK(does_match(g));
        generate(g);
        CHECK(g.a.current_value == 1744312007);
        CHECK(g.b.current_value == 137874439);
        CHECK(!does_match(g));
        generate(g);
        CHECK(g.a.current_value == 1352636452);
        CHECK(g.b.current_value == 285222916);
        CHECK(!does_match(g));
    }

    SECTION("Picky generate")
    {
        auto g = parseInput(input);
        CHECK(g.a.current_value == 65);
        CHECK(g.b.current_value == 8921);

        generate_picky(g);
        CHECK(g.a.current_value == 1352636452);
        CHECK(g.b.current_value == 1233683848);
        generate_picky(g);
        CHECK(g.a.current_value == 1992081072);
        CHECK(g.b.current_value == 862516352);
        generate_picky(g);
        CHECK(g.a.current_value == 530830436);
        CHECK(g.b.current_value == 1159784568);
        generate_picky(g);
        CHECK(g.a.current_value == 1980017072);
        CHECK(g.b.current_value == 1616057672);
        generate_picky(g);
        CHECK(g.a.current_value == 740335192);
        CHECK(g.b.current_value == 412269392);

        int i = 5;
        bool b_does_match = does_match(g);
        while(!b_does_match) {
            generate_picky(g);
            b_does_match = does_match(g);
            ++i;
        }
        CHECK(i == 1056);
        CHECK(g.a.current_value == 1023762912);
        CHECK(g.b.current_value == 896885216);
    }
}
