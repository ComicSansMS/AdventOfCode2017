#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <firewall.hpp>

#include <algorithm>
#include <ostream>
#include <iostream>
#include <sstream>
#include <string>

std::ostream& operator<<(std::ostream& os, Firewall const& rhs)
{
    auto const max_depth = std::max_element(begin(rhs.scanners), end(rhs.scanners),
                                            [](auto const& lhs, auto const& rhs)
                                            {
                                                Firewall::Scanner def{ -1, 0 };
                                                return lhs.value_or(def).range < rhs.value_or(def).range;
                                            })->value().range;

    auto const has_scanner = [&rhs](int pos, int depth) -> char
        {
            if(!rhs.scanners[pos]) {
                return (depth == 0) ? '.' : ' ';
            } else if(rhs.scanners[pos]->position == depth) {
                return 'S';
            } else {
                return ' ';
            }
        };
    for(int depth=-1; depth<max_depth; ++depth) {
        for(int pos=0; pos<rhs.scanners.size(); ++pos) {
            if(depth == -1) {
                std::cout << ' ' << pos << ' ';
            } else if(depth == 0) {
                if(pos == rhs.player_position) {
                    std::cout << '(' << has_scanner(pos, depth) << ')';
                } else {
                    if(rhs.scanners[pos]) {
                        std::cout << '[' << has_scanner(pos, depth) << ']';
                    } else {
                        std::cout << '.' << has_scanner(pos, depth) << '.';
                    }
                }
            } else {
                if(rhs.scanners[pos] && depth < rhs.scanners[pos]->range) {
                    std::cout << '[' << has_scanner(pos, depth) << ']';
                } else {
                    std::cout << "   ";
                }
            }
            std::cout << ' ';
        }
        std::cout << "\n";
    }
    return os;
}

TEST_CASE("Firewall")
{
    char const* input = R"(0: 3
1: 2
4: 4
6: 4)";

    SECTION("Parse input")
    {
        auto firewall = parseInput(input);
        REQUIRE(firewall.scanners.size() == 7);
        CHECK(firewall.scanners[0]->range == 3);
        CHECK(firewall.scanners[0]->position == 0);
        CHECK(firewall.scanners[0]->direction == Firewall::Scanner::Direction::Down);
        CHECK(firewall.scanners[1]->range == 2);
        CHECK(firewall.scanners[1]->position == 0);
        CHECK(firewall.scanners[1]->direction == Firewall::Scanner::Direction::Down);
        CHECK(!firewall.scanners[2]);
        CHECK(!firewall.scanners[3]);
        CHECK(firewall.scanners[4]->range == 4);
        CHECK(firewall.scanners[4]->position == 0);
        CHECK(firewall.scanners[4]->direction == Firewall::Scanner::Direction::Down);
        CHECK(!firewall.scanners[5]);
        CHECK(firewall.scanners[6]->range == 4);
        CHECK(firewall.scanners[6]->position == 0);
        CHECK(firewall.scanners[6]->direction == Firewall::Scanner::Direction::Down);
        CHECK(firewall.player_position == -1);

        auto firewall2 = parseInput("219: 658787");
        REQUIRE(firewall2.scanners.size() == 220);
        CHECK(firewall2.scanners[219]->range == 658787);
    }

    SECTION("Simulate Step")
    {
        auto firewall = parseInput(input);

        INFO(firewall);
        CHECK(simulateStep(firewall, Player::Move));
        CHECK(firewall.player_position == 0);
        CHECK(firewall.scanners[0]->position == 1);
        CHECK(firewall.scanners[1]->position == 1);
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        CHECK(firewall.player_position == 1);
        CHECK(firewall.scanners[0]->position == 2);
        CHECK(firewall.scanners[1]->position == 0);
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        CHECK(firewall.player_position == 2);
        CHECK(firewall.scanners[0]->position == 1);
        CHECK(firewall.scanners[1]->position == 1);
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        INFO(firewall);
        CHECK(simulateStep(firewall, Player::Move));
        INFO(firewall);
        CHECK(!simulateStep(firewall, Player::Move));
        INFO(firewall);
    }

    SECTION("Simulate Step, not moving player")
    {
        auto firewall = parseInput(input);

        CHECK(!simulateStep(firewall, Player::Stop));
        CHECK(firewall.player_position == -1);
        CHECK(firewall.scanners[0]->position == 1);
        CHECK(firewall.scanners[1]->position == 1);
        CHECK(!simulateStep(firewall, Player::Stop));
        CHECK(firewall.player_position == -1);
        CHECK(firewall.scanners[0]->position == 2);
        CHECK(firewall.scanners[1]->position == 0);
        CHECK(!simulateStep(firewall, Player::Stop));
        CHECK(firewall.player_position == -1);
        CHECK(firewall.scanners[0]->position == 1);
        CHECK(firewall.scanners[1]->position == 1);
    }

    SECTION("Simulate full travesal")
    {
        auto const initial_firewall = parseInput(input);
        auto firewall1 = initial_firewall;
        CHECK(simulateTraversal(0, firewall1).penalty == 24);

        auto firewall2 = initial_firewall;
        CHECK(simulateTraversal(10, firewall2).penalty == 0);
    }

    SECTION("Smallest delay")
    {
        auto const firewall = parseInput(input);
        CHECK(findSmallestDelay(firewall) == 10);
    }
}
