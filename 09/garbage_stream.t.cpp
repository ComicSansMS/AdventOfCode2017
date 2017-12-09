#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <garbage_stream.hpp>

TEST_CASE("Garbage Stream")
{
    SECTION("Parse Stream")
    {
        auto sm1 = parseStream("{}");
        CHECK(sm1.inAcceptingState());
        CHECK(sm1.getScore() == 1);

        auto sm2 = parseStream("{{{}}}");
        CHECK(sm2.inAcceptingState());
        CHECK(sm2.getScore() == 6);

        auto sm3 = parseStream("{{},{}}");
        CHECK(sm3.inAcceptingState());
        CHECK(sm3.getScore() == 5);

        auto sm4 = parseStream("{{{}{}}");
        CHECK(!sm4.inAcceptingState());

        auto sm5 = parseStream("{{{},{},{{}}}}");
        CHECK(sm5.inAcceptingState());
        CHECK(sm5.getScore() == 16);

        auto sm6 = parseStream("{<a>,<a>,<a>,<a>}");
        CHECK(sm6.inAcceptingState());
        CHECK(sm6.getScore() == 1);

        auto sm7 = parseStream("{{<ab>},{<ab>},{<ab>},{<ab>}}");
        CHECK(sm7.inAcceptingState());
        CHECK(sm7.getScore() == 9);

        auto sm8 = parseStream("{{<!!>},{<!!>},{<!!>},{<!!>}}");
        CHECK(sm8.inAcceptingState());
        CHECK(sm8.getScore() == 9);

        auto sm9 = parseStream("{{<a!>},{<a!>},{<a!>},{<ab>}}");
        CHECK(sm9.inAcceptingState());
        CHECK(sm9.getScore() == 3);
    }

    SECTION("Invalid streams")
    {
        CHECK_THROWS_AS(parseStream("<>"), unexpected_token);
        CHECK_THROWS_AS(parseStream("{>}"), unexpected_token);
    }

    SECTION("Count Garbage")
    {
        auto sm1 = parseStream("{<>}");
        CHECK(sm1.inAcceptingState());
        CHECK(sm1.getGarbageCount() == 0);

        auto sm2 = parseStream("{<<<<>}");
        CHECK(sm2.inAcceptingState());
        CHECK(sm2.getGarbageCount() == 3);

        auto sm3 = parseStream("{<{!>}>}");
        CHECK(sm3.inAcceptingState());
        CHECK(sm3.getGarbageCount() == 2);

        auto sm4 = parseStream("{<!!>}");
        CHECK(sm4.inAcceptingState());
        CHECK(sm4.getGarbageCount() == 0);

        auto sm5 = parseStream("{<!!!>>}");
        CHECK(sm5.inAcceptingState());
        CHECK(sm5.getGarbageCount() == 0);

        auto sm6 = parseStream(R"({<{o"i!a,<{i<a>})");
        CHECK(sm6.inAcceptingState());
        CHECK(sm6.getGarbageCount() == 10);
    }
}
