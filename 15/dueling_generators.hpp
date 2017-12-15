#ifndef ADVENT_OF_CODE_15_DUELING_GENERATORS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_DUELING_GENERATORS_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <vector>

struct Generator {
    int const factor;
    int current_value;
};

std::vector<Generator> parseInput(std::string_view input);

void generate(std::vector<Generator>& gen);

bool does_match(std::vector<Generator> const& gen);

#endif
