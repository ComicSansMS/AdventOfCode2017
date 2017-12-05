#ifndef ADVENT_OF_CODE_03_SPIRALSQUARE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_SPIRALSQUARE_HPP_INCLUDE_GUARD

#include <iosfwd>

/** Coordinate origin is at 1. X axis is to the right, Y upwards.
 * Example: Coordinates for:
 *   8: (0, -1)
 *  11: (2, 0)
 *  18: (-2, 1)
 */
struct SpiralCoordinate {
    int x;
    int y;
};

inline bool operator==(SpiralCoordinate const& lhs, SpiralCoordinate const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::ostream& operator<<(std::ostream& os, SpiralCoordinate const& rhs);

/** The spiral is formed of rings of odd numbers: 1, 3, 5, 7...
 * The complete spiral up to that ring is made up of the ring^2 numbers: 1, 9, 25, 49...
 */
int getRing(int n);

//SpiralCoordinate getCoordinate(int n);

int getDistance(int n);


#endif
