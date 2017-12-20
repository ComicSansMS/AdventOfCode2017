#ifndef ADVENT_OF_CODE_20_PARTICLES_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_20_PARTICLES_HPP_INCLUDE_GUARD

#include <cstdint>
#include <functional>
#include <string_view>
#include <vector>

struct Vec3 {
    std::int64_t x,y,z;

    Vec3& operator+=(Vec3 const& rhs);
};

inline bool operator==(Vec3 const& lhs, Vec3 const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

namespace std {
    template<>
    struct hash<Vec3> {
        std::size_t operator()(const Vec3 &k) const {
            return std::hash<std::int64_t>()(k.x)
                   ^ std::hash<std::int64_t>()(k.y)
                   ^ std::hash<std::int64_t>()(k.z);
        }
    };
}

struct Particle {
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
};

using Particles = std::vector<Particle>;

Particles parseInput(std::string_view input);

std::size_t simulate(Particles& particles);

void resolveCollisions(Particles& particles);

#endif
