#ifndef ADVENT_OF_CODE_16_DANCING_PROGRAMS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_DANCING_PROGRAMS_HPP_INCLUDE_GUARD

#include <string_view>
#include <variant>
#include <vector>

using DancingStage = std::vector<char>;

DancingStage createDancingStage();

std::string to_string(DancingStage const& stage);

struct Spin {
    std::size_t x;
};

struct Exchange {
    std::size_t pos_a;
    std::size_t pos_b;
};

struct Partner {
    char name_a;
    char name_b;
};

using DanceMove = std::variant<Spin, Exchange, Partner>;

using Dance = std::vector<DanceMove>;

Dance parseInput(std::string_view input);

void dance(DancingStage& stage, Spin const& move);
void dance(DancingStage& stage, Exchange const& move);
void dance(DancingStage& stage, Partner const& move);

void dance(DancingStage& stage, Dance const& dance);

std::size_t findPeriod(DancingStage& stage, Dance const& dance);
#endif
