#include <virus.hpp>

#include <cassert>
#include <sstream>
#include <string>
#include <utility>

Grid parseInput(std::string_view input)
{
    Grid ret;
    ret.position.x = 0;
    ret.position.y = 0;
    ret.direction = Direction::Up;

    std::vector<char> field;
    std::size_t line_width = 0;
    std::stringstream sstr(std::string{input});
    std::string line;
    bool is_first = true;
    while(std::getline(sstr, line)) {
        if(is_first) { line_width = line.length(); is_first = false; }
        assert(line.length() == line_width);
        field.insert(end(field), begin(line), end(line));
    }
    assert(field.size() % line_width == 0);
    assert(field.size() / line_width == line_width);
    assert((line_width % 2) == 1);

    int origin_offset = static_cast<int>(line_width / 2);
    std::size_t linear_count = 0;
    for(int y = origin_offset; y >= -origin_offset; --y) {
        for(int x = -origin_offset; x <= origin_offset; ++x) {
            char const c = field[linear_count];
            assert((c == '#') || (c == '.'));
            if(c == '#') {
                ret.field.insert(std::make_pair(Position{x, y}, State::Infected));
            }
            ++linear_count;
        }
    }

    return ret;
}

namespace {
Direction turnLeft(Direction dir)
{
    if(dir == Direction::Up) {
        return Direction::Left;
    } else if(dir == Direction::Down) {
        return Direction::Right;
    } else if(dir == Direction::Left) {
        return Direction::Down;
    } else {
        assert(dir == Direction::Right);
        return Direction::Up;
    }
}

Direction turnRight(Direction dir)
{
    if(dir == Direction::Up) {
        return Direction::Right;
    } else if(dir == Direction::Down) {
        return Direction::Left;
    } else if(dir == Direction::Left) {
        return Direction::Up;
    } else {
        assert(dir == Direction::Right);
        return Direction::Down;
    }
}

Position moveForward(Position p, Direction dir)
{
    if(dir == Direction::Up) {
        return Position{ p.x, p.y + 1 };
    } else if(dir == Direction::Down) {
        return Position{ p.x, p.y - 1 };
    } else if(dir == Direction::Left) {
        return Position{ p.x - 1, p.y };
    } else {
        assert(dir == Direction::Right);
        return Position{ p.x + 1, p.y };
    }
}
}

bool performBurst(Grid& grid)
{
    bool ret;
    auto it = grid.field.find(grid.position);
    if(it != end(grid.field)) {
        ret = false;
        grid.direction = turnRight(grid.direction);
        grid.field.erase(it);
    } else {
        ret = true;
        grid.direction = turnLeft(grid.direction);
        grid.field.insert(std::make_pair(grid.position, State::Infected));
    }
    grid.position = moveForward(grid.position, grid.direction);
    return ret;
}

int run(Grid& grid, int n_bursts)
{
    int ret = 0;
    for(int i=0; i<n_bursts; ++i) {
        if(performBurst(grid)) { ++ret; }
    }
    return ret;
}

bool performBurst2(Grid& grid)
{
    bool ret = false;
    auto it = grid.field.find(grid.position);
    if(it != end(grid.field)) {
        if(it->second == State::Weakened) {
            it->second = State::Infected;
            ret = true;
        } else if(it->second == State::Infected) {
            grid.direction = turnRight(grid.direction);
            it->second = State::Flagged;
        } else {
            assert(it->second == State::Flagged);
            grid.direction = turnRight(turnRight(grid.direction));
            grid.field.erase(it);
        }
    } else {
        // clean
        grid.direction = turnLeft(grid.direction);
        grid.field.insert(std::make_pair(grid.position, State::Weakened));
    }
    grid.position = moveForward(grid.position, grid.direction);
    return ret;
}

int run2(Grid& grid, int n_bursts)
{
    int ret = 0;
    for(int i=0; i<n_bursts; ++i) {
        if(performBurst2(grid)) { ++ret; }
    }
    return ret;
}
