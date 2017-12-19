#ifndef ADVENT_OF_CODE_19_TUBE_TRAVERSER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_19_TUBE_TRAVERSER_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct Coordinates {
    std::size_t x;
    std::size_t y;
};

inline bool operator==(Coordinates const& lhs, Coordinates const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

class Maze {
private:
    std::vector<char> m_maze;
    std::size_t m_stride;
public:
    Maze(std::string_view input);

    char getChar(Coordinates c) const;

    Coordinates findEntry() const;

    std::pair<std::string, std::size_t> traverse() const;
};

#endif
