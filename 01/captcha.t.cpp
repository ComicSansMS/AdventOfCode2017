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


TEST_CASE("Captcha 2")
{
    SECTION("Cyclic Access")
    {
        auto dseq = parseInput("1659");
        REQUIRE(dseq.size() == 4);
        CHECK(cyclic_access(dseq, 0).get() == 1);
        CHECK(cyclic_access(dseq, 1).get() == 6);
        CHECK(cyclic_access(dseq, 2).get() == 5);
        CHECK(cyclic_access(dseq, 3).get() == 9);
        CHECK(cyclic_access(dseq, 4).get() == 1);
        CHECK(cyclic_access(dseq, 5).get() == 6);
        CHECK(cyclic_access(dseq, 6).get() == 5);
        CHECK(cyclic_access(dseq, 7).get() == 9);
        CHECK(cyclic_access(dseq, 8).get() == 1);
        CHECK(cyclic_access(dseq, 9).get() == 6);
    }

    SECTION("Empty Input")
    {
        CHECK(solveCaptcha2(parseInput("")) == 0);
    }

    SECTION("Single Element Input")
    {
        CHECK(solveCaptcha2(parseInput("5")) == 5);
    }

    SECTION("First Input")
    {
        CHECK(solveCaptcha2(parseInput("1212")) == 6);
    }

    SECTION("Second Input")
    {
        CHECK(solveCaptcha2(parseInput("1221")) == 0);
    }

    SECTION("Third Input")
    {
        CHECK(solveCaptcha2(parseInput("123425")) == 4);
    }

    SECTION("Fourth Input")
    {
        CHECK(solveCaptcha2(parseInput("123123")) == 12);
    }

    SECTION("Fifth Input")
    {
        CHECK(solveCaptcha2(parseInput("12131415")) == 4);
    }
}
