#ifndef ADVENT_OF_CODE_05_JUMPER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_05_JUMPER_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

std::vector<int> parseInput(std::string_view input);

int runJumperVM(std::vector<int>& tape);

int runStrangeJumperVM(std::vector<int>& tape);

#endif
