#include <catch.hpp>

#include <fractal.hpp>
#include <iostream>

TEST_CASE("Fractal Art")
{
    char const* input = R"(../.# => ##./#../...
.#./..#/### => #..#/..../..../#..#
)";

    SECTION("Parse input")
    {
        auto rules = parseInput(input);
        REQUIRE(rules.rules2x2.size() == 1);
        CHECK(rules.rules2x2[0].match[0] == std::array<char, 4>{ 0, 0, 0, 1});
        CHECK(rules.rules2x2[0].replacement == std::array<char, 9>{ 1, 1, 0,
                                                                    1, 0, 0,
                                                                    0, 0, 0 });

        REQUIRE(rules.rules3x3.size() == 1);
        CHECK(rules.rules3x3[0].match[0] == std::array<char, 9>{ 0, 1, 0,
                                                                 0, 0, 1,
                                                                 1, 1, 1 });
        CHECK(rules.rules3x3[0].replacement == std::array<char, 16>{ 1, 0, 0, 1,
                                                                     0, 0, 0, 0,
                                                                     0, 0, 0, 0,
                                                                     1, 0, 0, 1 });
    }

    SECTION("Transformed rules 2x2")
    {
        Rules rules;
        Rule2x2 r2x2;
        r2x2.match[0] = std::array<char, 4>{ 1, 2, 3, 4 };
        rules.rules2x2.push_back(r2x2);
        addTransformedRules(rules);
        // Identity
        CHECK(rules.rules2x2[0].match[0] == std::array<char, 4>{ 1, 2,
                                                                 3, 4});
        // Rot90
        CHECK(rules.rules2x2[0].match[1] == std::array<char, 4>{ 3, 1,
                                                                 4, 2});
        // Rot180
        CHECK(rules.rules2x2[0].match[2] == std::array<char, 4>{ 4, 3,
                                                                 2, 1});
        // Rot270
        CHECK(rules.rules2x2[0].match[3] == std::array<char, 4>{ 2, 4,
                                                                 1, 3});
        // FlipH
        CHECK(rules.rules2x2[0].match[4] == std::array<char, 4>{ 2, 1,
                                                                 4, 3});
        // FlipV
        CHECK(rules.rules2x2[0].match[5] == std::array<char, 4>{ 3, 4,
                                                                 1, 2});
    }

    SECTION("Transformed rules 3x3")
    {
        Rules rules;
        Rule3x3 r3x3;
        r3x3.match[0] = std::array<char, 9>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        rules.rules3x3.push_back(r3x3);
        addTransformedRules(rules);
        // Identity
        CHECK(rules.rules3x3[0].match[0] == std::array<char, 9>{ 1, 2, 3,
                                                                 4, 5, 6,
                                                                 7, 8, 9 });
        // Rot90
        CHECK(rules.rules3x3[0].match[1] == std::array<char, 9>{ 7, 4, 1,
                                                                 8, 5, 2,
                                                                 9, 6, 3 });
        // Rot180
        CHECK(rules.rules3x3[0].match[2] == std::array<char, 9>{ 9, 8, 7,
                                                                 6, 5, 4,
                                                                 3, 2, 1 });
        // Rot270
        CHECK(rules.rules3x3[0].match[3] == std::array<char, 9>{ 3, 6, 9,
                                                                 2, 5, 8,
                                                                 1, 4, 7 });
        // FlipH
        CHECK(rules.rules3x3[0].match[4] == std::array<char, 9>{ 3, 2, 1,
                                                                 6, 5, 4,
                                                                 9, 8, 7 });
        // FlipV
        CHECK(rules.rules3x3[0].match[5] == std::array<char, 9>{ 7, 8, 9,
                                                                 4, 5, 6,
                                                                 1, 2, 3 });
    }

    SECTION("Canvas Construction")
    {
        Canvas c;
        CHECK(c.getWidth() == 3);

        CHECK(!c.getField(0, 0));
        CHECK(c.getField(1, 0));
        CHECK(!c.getField(2, 0));

        CHECK(!c.getField(0, 1));
        CHECK(!c.getField(1, 1));
        CHECK(c.getField(2, 1));

        CHECK(c.getField(0, 2));
        CHECK(c.getField(1, 2));
        CHECK(c.getField(2, 2));
    }

    SECTION("Canvas custom construction")
    {
        Canvas c(std::vector<char>{ 0, 0, 0, 1,
                                    1, 0, 1, 0,
                                    0, 0, 1, 1,
                                    1, 0, 0, 0 }, 4);
        CHECK(c.getWidth() == 4);

        CHECK(!c.getField(0, 0));
        CHECK(!c.getField(1, 0));
        CHECK(!c.getField(2, 0));
        CHECK(c.getField(3, 0));

        CHECK(c.getField(0, 1));
        CHECK(!c.getField(1, 1));
        CHECK(c.getField(2, 1));
        CHECK(!c.getField(3, 1));

        CHECK(!c.getField(0, 2));
        CHECK(!c.getField(1, 2));
        CHECK(c.getField(2, 2));
        CHECK(c.getField(3, 2));

        CHECK(c.getField(0, 3));
        CHECK(!c.getField(1, 3));
        CHECK(!c.getField(2, 3));
        CHECK(!c.getField(3, 3));
    }

    SECTION("Canvas 2x2 subfield")
    {
        Canvas c(std::vector<char>{ 0, 0, 0, 1,
                                    1, 0, 1, 0,
                                    0, 0, 1, 1,
                                    1, 0, 0, 0 }, 4);

        auto ul = c.getSubField2x2(0, 0);
        CHECK(!ul.getField(0, 0));
        CHECK(!ul.getField(1, 0));
        CHECK(ul.getField(0, 1));
        CHECK(!ul.getField(1, 1));

        auto ur = c.getSubField2x2(1, 0);
        CHECK(!ur.getField(0, 0));
        CHECK(ur.getField(1, 0));
        CHECK(ur.getField(0, 1));
        CHECK(!ur.getField(1, 1));

        auto ll = c.getSubField2x2(0, 1);
        CHECK(!ll.getField(0, 0));
        CHECK(!ll.getField(1, 0));
        CHECK(ll.getField(0, 1));
        CHECK(!ll.getField(1, 1));

        auto lr = c.getSubField2x2(1, 1);
        CHECK(lr.getField(0, 0));
        CHECK(lr.getField(1, 0));
        CHECK(!lr.getField(0, 1));
        CHECK(!lr.getField(1, 1));
    }

    SECTION("Canvas 3x3 subfield")
    {
        Canvas c(std::vector<char>{ 0, 0, 0, 1, 1, 1,
                                    1, 0, 1, 0, 1, 0,
                                    0, 0, 1, 1, 1, 1,
                                    1, 0, 0, 0, 0, 1,
                                    0, 0, 1, 1, 0, 0,
                                    1, 1, 1, 1, 1, 1 }, 6);

        auto ul = c.getSubField3x3(0, 0);
        CHECK(!ul.getField(0, 0));
        CHECK(!ul.getField(1, 0));
        CHECK(!ul.getField(2, 0));
        CHECK(ul.getField(0, 1));
        CHECK(!ul.getField(1, 1));
        CHECK(ul.getField(2, 1));
        CHECK(!ul.getField(0, 2));
        CHECK(!ul.getField(1, 2));
        CHECK(ul.getField(2, 2));

        auto ur = c.getSubField3x3(1, 0);
        CHECK(ur.getField(0, 0));
        CHECK(ur.getField(1, 0));
        CHECK(ur.getField(2, 0));
        CHECK(!ur.getField(0, 1));
        CHECK(ur.getField(1, 1));
        CHECK(!ur.getField(2, 1));
        CHECK(ur.getField(0, 2));
        CHECK(ur.getField(1, 2));
        CHECK(ur.getField(2, 2));

        auto ll = c.getSubField3x3(0, 1);
        CHECK(ll.getField(0, 0));
        CHECK(!ll.getField(1, 0));
        CHECK(!ll.getField(2, 0));
        CHECK(!ll.getField(0, 1));
        CHECK(!ll.getField(1, 1));
        CHECK(ll.getField(2, 1));
        CHECK(ll.getField(0, 2));
        CHECK(ll.getField(1, 2));
        CHECK(ll.getField(2, 2));

        auto lr = c.getSubField3x3(1, 1);
        CHECK(!lr.getField(0, 0));
        CHECK(!lr.getField(1, 0));
        CHECK(lr.getField(2, 0));
        CHECK(lr.getField(0, 1));
        CHECK(!lr.getField(1, 1));
        CHECK(!lr.getField(2, 1));
        CHECK(lr.getField(0, 2));
        CHECK(lr.getField(1, 2));
        CHECK(lr.getField(2, 2));
    }

    SECTION("Matching Rule2x2")
    {
        auto const rule = parseInput("#./.. => .../.../...").rules2x2[0];
        std::vector<char> field = { 0, 0,    1, 0,    0, 1,
                                    0, 0,    0, 0,    0, 0,

                                    0, 0,    0, 0,    1, 0,
                                    0, 1,    1, 0,    0, 1,

                                    1, 1,    1, 0,    1, 1,
                                    0, 1,    1, 1,    1, 1, };
        Canvas c(field, 6);

        CHECK(!doesMatch(rule, c.getSubField2x2(0, 0)));
        CHECK(doesMatch(rule, c.getSubField2x2(1, 0)));
        CHECK(doesMatch(rule, c.getSubField2x2(2, 0)));
        CHECK(doesMatch(rule, c.getSubField2x2(0, 1)));
        CHECK(doesMatch(rule, c.getSubField2x2(1, 1)));
        CHECK(!doesMatch(rule, c.getSubField2x2(2, 1)));
        CHECK(!doesMatch(rule, c.getSubField2x2(0, 2)));
        CHECK(!doesMatch(rule, c.getSubField2x2(1, 2)));
        CHECK(!doesMatch(rule, c.getSubField2x2(2, 2)));
    }

    SECTION("Matching Rule3x3")
    {
        auto const rule = parseInput("##./#../... => ..../..../..../....").rules3x3[0];

        std::vector<char> field = { 0, 0, 0,    1, 1, 0,    0, 1, 1,
                                    0, 0, 0,    1, 0, 0,    0, 0, 1,
                                    0, 0, 0,    0, 0, 0,    0, 0, 0,

                                    0, 0, 0,    0, 0, 0,    1, 0, 0,
                                    0, 0, 1,    1, 0, 0,    1, 0, 0,
                                    0, 1, 1,    1, 1, 0,    1, 0, 0,

                                    1, 1, 0,    0, 0, 0,    0, 1, 0,
                                    1, 1, 0,    0, 0, 0,    1, 1, 1,
                                    0, 0, 0,    1, 1, 1,    0, 0, 0 };

        Canvas c(field, 9);

        CHECK(!doesMatch(rule, c.getSubField3x3(0, 0)));
        CHECK(doesMatch(rule, c.getSubField3x3(1, 0)));
        CHECK(doesMatch(rule, c.getSubField3x3(2, 0)));
        CHECK(doesMatch(rule, c.getSubField3x3(0, 1)));
        CHECK(doesMatch(rule, c.getSubField3x3(1, 1)));
        CHECK(!doesMatch(rule, c.getSubField3x3(2, 1)));
        CHECK(!doesMatch(rule, c.getSubField3x3(0, 2)));
        CHECK(!doesMatch(rule, c.getSubField3x3(1, 2)));
        CHECK(!doesMatch(rule, c.getSubField3x3(2, 2)));
    }

    SECTION("Apply rules")
    {
        auto const rules = parseInput(R"(../.# => ##./#../...
.#./..#/### => #..#/..../..../#..#
)");
        Canvas c;

        std::cout << c << '\n';

        REQUIRE(c.getWidth() == 3);
        c = c.applyRules(rules);
        REQUIRE(c.getWidth() == 4);
        CHECK(c.getField(0,0));
        CHECK(c.getField(0,3));
        CHECK(c.getField(3,0));
        CHECK(c.getField(3,3));

        CHECK(!c.getField(0,1));
        CHECK(!c.getField(0,2));
        CHECK(!c.getField(1,0));
        CHECK(!c.getField(1,1));
        CHECK(!c.getField(1,2));
        CHECK(!c.getField(1,3));
        CHECK(!c.getField(2,0));
        CHECK(!c.getField(2,1));
        CHECK(!c.getField(2,2));
        CHECK(!c.getField(2,3));
        CHECK(!c.getField(3,1));
        CHECK(!c.getField(3,2));

        std::cout << c << '\n';

        c = c.applyRules(rules);

        for(int iy=0; iy<2; ++iy) {
            for(int ix=0; ix<2; ++ix) {
                auto block = c.getSubField3x3(ix, iy);
                CHECK(block.getField(0, 0));
                CHECK(block.getField(1, 0));
                CHECK(!block.getField(2, 0));
                CHECK(block.getField(0, 1));
                CHECK(!block.getField(1, 1));
                CHECK(!block.getField(2, 1));
                CHECK(!block.getField(0, 2));
                CHECK(!block.getField(1, 2));
                CHECK(!block.getField(2, 2));
            }
        }
        std::cout << c << '\n';
    }

    SECTION("Transform & count")
    {
        auto const rules = parseInput(R"(../.# => ##./#../...
.#./..#/### => #..#/..../..../#..#
)");
        Canvas c;
        c = c.applyRules(rules);
        c = c.applyRules(rules);
        CHECK(c.pegCount() == 12);
    }
}
