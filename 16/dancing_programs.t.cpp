#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <dancing_programs.hpp>

TEST_CASE("Dancing programs")
{
    char const* input = "s1,x3/4,pe/b";

    SECTION("Create dancing stage")
    {
        auto stage = createDancingStage();
        REQUIRE(stage.size() == 16);
        for(int i=0; i<16; ++i)
        {
            CHECK(stage[i] == 'a' + i);
        }
    }

    SECTION("Stage To String")
    {
        auto stage = createDancingStage();
        CHECK(to_string(stage) == "abcdefghijklmnop");
    }

    SECTION("Parse input")
    {
        auto dance = parseInput(input);

        REQUIRE(dance.size() == 3);

        CHECK(std::get<Spin>(dance[0]).x == 1);

        CHECK(std::get<Exchange>(dance[1]).pos_a == 3);
        CHECK(std::get<Exchange>(dance[1]).pos_b == 4);

        CHECK(std::get<Partner>(dance[2]).name_a == 'e');
        CHECK(std::get<Partner>(dance[2]).name_b == 'b');
    }

    SECTION("Spin")
    {
        auto stage = createDancingStage();
        stage.resize(5);
        Spin s3{ 3 };
        dance(stage, s3);
        CHECK(to_string(stage) == "cdeab");

        stage = createDancingStage();
        stage.resize(5);
        Spin s1{ 1 };
        dance(stage, s1);
        CHECK(to_string(stage) == "eabcd");
    }

    SECTION("Exchange")
    {
        auto stage = createDancingStage();
        stage.resize(5);
        Exchange x2_4{ 2, 4 };
        dance(stage, x2_4);
        CHECK(to_string(stage) == "abedc");
    }

    SECTION("Partner")
    {
        auto stage = createDancingStage();
        stage.resize(5);
        Partner pe_b{ 'e', 'b' };
        dance(stage, pe_b);
        CHECK(to_string(stage) == "aecdb");
    }

    SECTION("Dance")
    {
        auto full_dance = parseInput(input);
        auto stage = createDancingStage();
        stage.resize(5);

        dance(stage, full_dance);
        CHECK(to_string(stage) == "baedc");

        dance(stage, full_dance);
        CHECK(to_string(stage) == "ceadb");
    }

    SECTION("Find period")
    {
        auto full_dance = parseInput(input);
        auto stage = createDancingStage();
        stage.resize(5);

        CHECK(findPeriod(stage, full_dance) == 4);
        CHECK(findPeriod(stage, full_dance) == 4);
    }
}
