#include <spiralsquare.hpp>

#include <ostream>

std::ostream& operator<<(std::ostream& os, SpiralCoordinate const& rhs)
{
    return os << "{" << rhs.x << ", " << rhs.y << "}";
}

int getRing(int n)
{
    int ring = 1;
    while(ring * ring < n) { ring += 2; }
    return ring;
}

int getDistance(int n)
{
    if(n == 1) return 0;
    int const ring = getRing(n);
    int const ring_diameter = (ring - 1) / 2;
    int const lower_ring = ring - 2;
    int const lower_min_distance = (lower_ring - 1) / 2;

    bool descending = (ring != 3);
    int additional_distance = ring_diameter;
    for(int i = lower_ring*lower_ring + 1; i < n; ++i) {
        if(descending) {
            --additional_distance;
        } else {
            ++additional_distance;
        }
        if(additional_distance == 1) { descending = false; }
        else if(additional_distance > ring_diameter) { descending = true; }
    }

    return lower_min_distance + additional_distance;
}
