#ifndef ADVENT_OF_CODE_11_HEXGRID_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_HEXGRID_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>
#include <iosfwd>


enum class Direction {
    N,
    NE,
    SE,
    S,
    SW,
    NW
};

using Path = std::vector<Direction>;

Path parseInput(std::string_view input);

struct CubicCoordinates {
    int x;
    int y;
    int z;

    CubicCoordinates();
    CubicCoordinates(int nx, int ny, int nz);
};

bool operator==(CubicCoordinates const& lhs, CubicCoordinates const& rhs);
bool operator!=(CubicCoordinates const& lhs, CubicCoordinates const& rhs);
std::ostream& operator<<(std::ostream& os, CubicCoordinates const& rhs);

CubicCoordinates takeStep(CubicCoordinates const& start, Direction direction);

int distance(CubicCoordinates const& a, CubicCoordinates const& b);

int distanceFromOrigin(CubicCoordinates const& cc);

CubicCoordinates walkPath(CubicCoordinates const& start, Path const& path);

std::vector<CubicCoordinates> getPath(CubicCoordinates const& start, Path const& path);

#endif
