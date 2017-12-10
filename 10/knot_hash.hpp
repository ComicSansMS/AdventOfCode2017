#ifndef ADVENT_OF_CODE_10_KNOT_HASH_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_KNOT_HASH_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

using StringCircle = std::vector<int>;

std::vector<int> parseLengths(std::string_view input);

std::vector<int> parseLengths_ASCII(std::string_view input);

StringCircle buildInitialCircle();

void circularReverse(StringCircle& sc, int start_index, int length);

struct KnotHashState {
    int current_position;
    int skip_size;
};

void knotHashSingleRound(StringCircle& sc, std::vector<int> const& lengths, KnotHashState& state);

std::vector<int> getDenseHash(StringCircle& sc);

std::string hexify(std::vector<int> const& dense_hash);

std::string knotHash(std::string_view input);

#endif
