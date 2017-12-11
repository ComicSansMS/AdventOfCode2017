#include <hexgrid.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <ostream>

Path parseInput(std::string_view input)
{
    auto const string_to_direction = [](char const* buffer) {
            std::optional<Direction> ret;
            if(std::strcmp(buffer, "n") == 0) {
                ret = Direction::N;
            } else if(std::strcmp(buffer, "ne") == 0) {
                ret = Direction::NE;
            } else if(std::strcmp(buffer, "se") == 0) {
                ret =  Direction::SE;
            } else if(std::strcmp(buffer, "s") == 0) {
                ret =  Direction::S;
            } else if(std::strcmp(buffer, "sw") == 0) {
                ret =  Direction::SW;
            } else if(std::strcmp(buffer, "nw") == 0) {
                ret =  Direction::NW;
            }
            return ret;
        };

    char buffer[3];
    int i = 0;
    Path ret;
    for(auto const& c : input) {
        assert(i < sizeof(buffer));
        if(c != ',') {
            buffer[i] = c;
            ++i;
        } else {
            buffer[i] = '\0';
            auto const direction = string_to_direction(buffer);
            assert(direction);
            ret.push_back(*direction);
            i = 0;
        }
    }
    buffer[i] = '\0';
    ret.push_back(string_to_direction(buffer).value());
    return ret;
}

CubicCoordinates::CubicCoordinates()
    :x(0), y(0), z(0)
{}

CubicCoordinates::CubicCoordinates(int nx, int ny, int nz)
    :x(nx), y(ny), z(nz)
{}

bool operator==(CubicCoordinates const& lhs, CubicCoordinates const& rhs)
{
    return (lhs.x == rhs.x) &&
           (lhs.y == rhs.y) &&
           (lhs.z == rhs.z);
}

bool operator!=(CubicCoordinates const& lhs, CubicCoordinates const& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, CubicCoordinates const& rhs)
{
    return os << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
}

CubicCoordinates takeStep(CubicCoordinates const& start, Direction direction)
{
    CubicCoordinates ret;
    switch(direction)
    {
    case Direction::N:
        ret.x = start.x;
        ret.y = start.y + 1;
        ret.z = start.z - 1;
        break;
    case Direction::NE:
        ret.x = start.x + 1;
        ret.y = start.y;
        ret.z = start.z - 1;
        break;
    case Direction::SE:
        ret.x = start.x + 1;
        ret.y = start.y - 1;
        ret.z = start.z;
        break;
    case Direction::S:
        ret.x = start.x;
        ret.y = start.y - 1;
        ret.z = start.z + 1;
        break;
    case Direction::SW:
        ret.x = start.x - 1;
        ret.y = start.y;
        ret.z = start.z + 1;
        break;
    case Direction::NW:
        ret.x = start.x - 1;
        ret.y = start.y + 1;
        ret.z = start.z;
        break;
    }
    return ret;
}

int distance(CubicCoordinates const& a, CubicCoordinates const& b)
{
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z)) / 2;
}

int distanceFromOrigin(CubicCoordinates const& cc)
{
    return distance(CubicCoordinates(), cc);
}

CubicCoordinates walkPath(CubicCoordinates const& start, Path const& path)
{
    CubicCoordinates pos = start;
    for(auto const& step : path) {
        pos = takeStep(pos, step);
    }
    return pos;
}

std::vector<CubicCoordinates> getPath(CubicCoordinates const& start, Path const& path)
{
    std::vector<CubicCoordinates> ret;
    ret.reserve(path.size() + 1);
    ret.push_back(start);
    for(auto const& step : path) {
        ret.push_back(takeStep(ret.back(), step));
    }
    return ret;
}
