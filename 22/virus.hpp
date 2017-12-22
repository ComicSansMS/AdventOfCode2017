#ifndef ADVENT_OF_CODE_22_VIRUS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_22_VIRUS_HPP_INCLUDE_GUARD

#include <cstdint>
#include <functional>
#include <string_view>
#include <unordered_map>

struct Position {
    std::int32_t x;
    std::int32_t y;
};

inline bool operator==(Position const& lhs, Position const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

namespace std {
template<>
struct hash<Position> {
    std::size_t operator()(Position const& k) const {
        return std::hash<decltype(k.x)>()(k.x) ^ std::hash<decltype(k.y)>()(k.y << 16);
    }
};
}

enum class Direction {
    Up, Down, Left, Right
};

enum class State {
    Weakened, Infected, Flagged
};

struct Grid{
    std::unordered_map<Position, State> field;
    Position position;
    Direction direction;
};

Grid parseInput(std::string_view input);

bool performBurst(Grid& grid);
int run(Grid& grid, int n_bursts);

bool performBurst2(Grid& grid);
int run2(Grid& grid, int n_bursts);

#endif
