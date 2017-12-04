#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <captcha.hpp>

TEST_CASE("Captcha")
{

    SECTION("Input Parsing")
    {
        auto dseq = parseInput("1659");
        REQUIRE(dseq.size() == 4);
        CHECK(dseq[0].get() == 1);
        CHECK(dseq[1].get() == 6);
        CHECK(dseq[2].get() == 5);
        CHECK(dseq[3].get() == 9);
    }

    SECTION("Empty Input")
    {
        CHECK(solveCaptcha(parseInput("")) == 0);
    }

    SECTION("Single Element Input")
    {
        CHECK(solveCaptcha(parseInput("5")) == 5);
    }

    SECTION("First Example")
    {
        CHECK(solveCaptcha(parseInput("1122")) == 3);
    }

    SECTION("Second Example")
    {
        CHECK(solveCaptcha(parseInput("1111")) == 4);
    }

    SECTION("Third Example")
    {
        CHECK(solveCaptcha(parseInput("1234")) == 0);
    }

    SECTION("Fourth Example")
    {
        CHECK(solveCaptcha(parseInput("91212129")) == 9);
    }
}

