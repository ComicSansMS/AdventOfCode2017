#ifndef ADVENT_OF_CODE_15_DUELING_GENERATORS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_DUELING_GENERATORS_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <utility>

struct Generator {
    int factor;
    int current_value;
};

struct Generators {
    Generator a;
    Generator b;
};

Generators parseInput(std::string_view input);

void generate(Generators& gen);

void generate_picky(Generators& gen);

bool does_match(Generators const& gen);

#endif
