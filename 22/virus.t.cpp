#include <catch.hpp>

#include <virus.hpp>

TEST_CASE("Sporifica Virus")
{
    char const* input = R"(..#
#..
...
)";

    SECTION("Parse input")
    {
        auto const grid = parseInput(input);

        CHECK(grid.position == Position{0, 0});
        CHECK(grid.direction == Direction::Up);
        CHECK(grid.field.size() == 2);
        CHECK(grid.field.find(Position{ 1, 1}) != end(grid.field));
        CHECK(grid.field.find(Position{-1, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{ 0, 0}) == end(grid.field));

        auto const grid2 = parseInput("#....\n#...#\n..#..\n.....\n#...#");
        CHECK(grid2.field.size() == 6);
        CHECK(grid2.field.find(Position{-2, 2 }) != end(grid2.field));
        CHECK(grid2.field.find(Position{-2, 1 }) != end(grid2.field));
        CHECK(grid2.field.find(Position{ 2, 1 }) != end(grid2.field));
        CHECK(grid2.field.find(Position{ 0, 0 }) != end(grid2.field));
        CHECK(grid2.field.find(Position{-2,-2 }) != end(grid2.field));
        CHECK(grid2.field.find(Position{ 2,-2 }) != end(grid2.field));
    }

    SECTION("Bursts")
    {
        auto grid = parseInput(input);

        CHECK(performBurst(grid));
        CHECK(grid.field.size() == 3);
        CHECK(grid.field.find(Position{ 0, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{-1, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{ 1, 1}) != end(grid.field));
        CHECK(grid.position == Position{-1, 0});
        CHECK(grid.direction == Direction::Left);

        CHECK(!performBurst(grid));
        CHECK(grid.field.size() == 2);
        CHECK(grid.field.find(Position{ 0, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{ 1, 1}) != end(grid.field));
        CHECK(grid.position == Position{-1, 1});
        CHECK(grid.direction == Direction::Up);

        CHECK(performBurst(grid));
        CHECK(performBurst(grid));
        CHECK(performBurst(grid));
        CHECK(performBurst(grid));
        CHECK(grid.field.size() == 6);
        CHECK(grid.field.find(Position{ 0, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{ 1, 1}) != end(grid.field));
        CHECK(grid.field.find(Position{-1, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{-2, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{-2, 1}) != end(grid.field));
        CHECK(grid.field.find(Position{-1, 1}) != end(grid.field));
        CHECK(grid.position == Position{-1, 1});
        CHECK(grid.direction == Direction::Up);

        CHECK(!performBurst(grid));
        CHECK(grid.field.size() == 5);
        CHECK(grid.field.find(Position{ 0, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{ 1, 1}) != end(grid.field));
        CHECK(grid.field.find(Position{-1, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{-2, 0}) != end(grid.field));
        CHECK(grid.field.find(Position{-2, 1}) != end(grid.field));
        CHECK(grid.position == Position{ 0, 1});
        CHECK(grid.direction == Direction::Right);
    }

    SECTION("Count infections")
    {
        auto const grid = parseInput(input);

        auto grid1 = grid;
        CHECK(run(grid1, 70) == 41);

        auto grid2 = grid;
        CHECK(run(grid2, 10'000) == 5587);
    }

    SECTION("Bursts 2")
    {
        auto grid = parseInput(input);

        CHECK(!performBurst2(grid));
        CHECK(grid.field.size() == 3);
        CHECK(grid.field.find(Position{ 0, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 0})->second == State::Infected);
        CHECK(grid.field.find(Position{ 1, 1})->second == State::Infected);
        CHECK(grid.position == Position{-1, 0});
        CHECK(grid.direction == Direction::Left);

        CHECK(!performBurst2(grid));
        CHECK(grid.field.size() == 3);
        CHECK(grid.field.find(Position{ 0, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 0})->second == State::Flagged);
        CHECK(grid.field.find(Position{ 1, 1})->second == State::Infected);
        CHECK(grid.position == Position{-1, 1});
        CHECK(grid.direction == Direction::Up);

        CHECK(!performBurst2(grid));
        CHECK(!performBurst2(grid));
        CHECK(!performBurst2(grid));
        CHECK(grid.field.size() == 6);
        CHECK(grid.field.find(Position{ 0, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 0})->second == State::Flagged);
        CHECK(grid.field.find(Position{ 1, 1})->second == State::Infected);
        CHECK(grid.field.find(Position{-2, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{-2, 1})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 1})->second == State::Weakened);
        CHECK(grid.position == Position{-1, 0});
        CHECK(grid.direction == Direction::Right);

        CHECK(!performBurst2(grid));
        CHECK(grid.field.size() == 5);
        CHECK(grid.field.find(Position{ 0, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{ 1, 1})->second == State::Infected);
        CHECK(grid.field.find(Position{-2, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{-2, 1})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 1})->second == State::Weakened);
        CHECK(grid.position == Position{-2, 0});
        CHECK(grid.direction == Direction::Left);

        CHECK(performBurst2(grid));
        CHECK(grid.field.size() == 5);
        CHECK(grid.field.find(Position{ 0, 0})->second == State::Weakened);
        CHECK(grid.field.find(Position{ 1, 1})->second == State::Infected);
        CHECK(grid.field.find(Position{-2, 0})->second == State::Infected);
        CHECK(grid.field.find(Position{-2, 1})->second == State::Weakened);
        CHECK(grid.field.find(Position{-1, 1})->second == State::Weakened);
        CHECK(grid.position == Position{-3, 0});
        CHECK(grid.direction == Direction::Left);
    }

    SECTION("Count infections 2")
    {
        auto const grid = parseInput(input);

        auto grid1 = grid;
        CHECK(run2(grid1, 100) == 26);
    }
}
