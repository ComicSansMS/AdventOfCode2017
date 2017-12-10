#ifndef ADVENT_OF_CODE_10_KNOT_HASH_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_KNOT_HASH_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

using StringCircle = std::vector<int>;

std::vector<int> parseLengths(std::string_view input);

void circularReverse(StringCircle& sc, int start_index, int length);

void knotHash(StringCircle& sc, std::vector<int> const& lengths);

#endif
