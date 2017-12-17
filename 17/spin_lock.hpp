#ifndef ADVENT_OF_CODE_17_SPIN_LOCK_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_SPIN_LOCK_HPP_INCLUDE_GUARD

#include <vector>
#include <utility>

using Buffer = std::vector<int>;

std::size_t spin_step(Buffer& buffer, int step_size, int current_step, std::size_t start_index);

std::pair<Buffer, std::size_t> spin(int step_size, int total_steps);

int spin_after_0(int step_size, int total_steps);

#endif
