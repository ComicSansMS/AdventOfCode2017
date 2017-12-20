#include <particles.hpp>

#include <cassert>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

Particles parseInput(std::string_view input)
{
    Particles ret;

    //std::regex line_rx(R"(^p=<([-\d]+),([-\d]+),([-\d]+)>, v=<([-\d]+),([-\d]+),([-\d]+)>, a=<([-\d]+), <([-\d]+),([-\d]+),([-\d]+)>$)");
    std::regex line_rx(R"(^p=<([-\d]+),([-\d]+),([-\d]+)>, v=<([-\d]+),([-\d]+),([-\d]+)>, a=<([-\d]+),([-\d]+),([-\d]+)>$)");

    std::stringstream sstr(std::string{input});
    std::string line;
    while(std::getline(sstr, line)) {
        std::smatch match;
        std::regex_match(line, match, line_rx);
        Vec3 const pos{ std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]) };
        Vec3 const vel{ std::stoi(match[4]), std::stoi(match[5]), std::stoi(match[6]) };
        Vec3 const acc{ std::stoi(match[7]), std::stoi(match[8]), std::stoi(match[9]) };
        ret.push_back(Particle{ pos, vel, acc });
    }

    return ret;
}

Vec3& Vec3::operator+=(Vec3 const& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

std::size_t simulate(Particles& particles)
{
    std::size_t closest = 0;
    std::int64_t closest_distance = std::numeric_limits<std::int64_t>::max();
    for(std::size_t i = 0; i < particles.size(); ++i) {
        auto& p = particles[i];
        p.velocity += p.acceleration;
        p.position += p.velocity;
        auto const distance = std::abs(p.position.x) + std::abs(p.position.y) + std::abs(p.position.z);
        if(distance < closest_distance) {
            closest = i;
            closest_distance = distance;
        }
    }
    return closest;
}

void resolveCollisions(Particles& particles)
{
    std::unordered_map<Vec3, std::size_t> position_count;
    for(auto const& p : particles) {
        ++position_count[p.position];
    }
    particles.erase(std::remove_if(begin(particles), end(particles),
                                   [&position_count](auto p) { return position_count.find(p.position)->second > 1; }),
                    end(particles));
}
