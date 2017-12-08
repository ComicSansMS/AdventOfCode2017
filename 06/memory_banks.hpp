#ifndef ADVENT_OF_CODE_06_MEMORY_BANKS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_MEMORY_BANKS_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

using MemoryArray = std::vector<int>;

MemoryArray parseInput(std::string_view input);

std::string banksToString(MemoryArray const& banks);

void iterate(MemoryArray& banks);

int redistribute(MemoryArray& banks);

#endif
