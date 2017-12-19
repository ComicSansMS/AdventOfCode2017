#include <tube_traverser.hpp>

#include <algorithm>
#include <cassert>

#include <sstream>
#include <string>

Maze::Maze(std::string_view input)
{
    auto const it_break = std::find(begin(input), end(input), '\n');
    assert(it_break != end(input));
    m_stride = it_break - begin(input);

    std::size_t const n_lines = (input.length() / (m_stride + 1));
    assert((input.length() % n_lines) == 0);
    m_maze.reserve(m_stride * n_lines);
    for(std::size_t i = 0; i < n_lines; ++i) {
        auto it_line_start = begin(input) + (i * m_stride) + i;
        m_maze.insert(m_maze.end(), it_line_start, it_line_start + m_stride);
    }
}

char Maze::getChar(Coordinates c) const
{
    assert((c.x < m_stride) && (c.y < m_maze.size() / m_stride));
    return m_maze[c.y * m_stride + c.x];
}

Coordinates Maze::findEntry() const
{
    auto const pos = static_cast<std::size_t>(std::find(begin(m_maze), end(m_maze), '|') - begin(m_maze));
    assert(pos < m_stride);
    return Coordinates{ pos, 0 };
}

std::pair<std::string, std::size_t> Maze::traverse() const
{
    enum class Direction {
        Up, Down, Left, Right
    };

    std::string ret;
    std::size_t steps = 0;
    Coordinates c = findEntry();
    Direction dir = Direction::Down;

    char f = getChar(c);
    bool done = false;
    while(f != ' ') {
        while(f != '+') {
            if(f >= 'A' && f <= 'Z') { ret.push_back(f); }
            if(dir == Direction::Up) { --c.y; }
            else if(dir == Direction::Down) { ++c.y; }
            else if(dir == Direction::Left) { --c.x; }
            else if(dir == Direction::Right) { ++c.x; }
            f = getChar(c);
            ++steps;
            if(f == ' ') { done = true; break; }
        }
        if(done) { break; }

        // switch direction
        if((dir == Direction::Up) || (dir == Direction::Down)) {
            if(getChar(Coordinates{c.x + 1, c.y}) != ' ') {
                dir = Direction::Right;
                ++c.x;
            } else {
                dir = Direction::Left;
                --c.x;
            }
        } else {
            if(getChar(Coordinates{c.x, c.y + 1}) != ' ') {
                dir = Direction::Down;
                ++c.y;
            } else {
                dir = Direction::Up;
                --c.y;
            }
        }
        f = getChar(c);
        ++steps;
    }

    return std::make_pair(ret, steps);
}
