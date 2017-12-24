#ifndef ADVENT_OF_CODE_24_BRIDGE_BUILDER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_24_BRIDGE_BUILDER_HPP_INCLUDE_GUARD

#include <array>
#include <string_view>
#include <vector>

using Port = std::array<int, 2>;

using Ports = std::vector<Port>;

Ports parseInput(std::string_view input);

std::vector<Ports> getBridges(Ports const& ports);

int getBridgeStrength(Ports const& bridge);

int getStrongestStrength(std::vector<Ports> const& bridges);

int getLongestStrongestStrength(std::vector<Ports> const& bridges);

#endif
