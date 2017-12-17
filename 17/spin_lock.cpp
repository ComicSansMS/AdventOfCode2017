#include <spin_lock.hpp>

#include <cassert>

std::size_t spin_step(Buffer& buffer, int step_size, int current_step, std::size_t start_index)
{
    std::size_t insert_index = ((start_index + step_size) % buffer.size()) + 1;
    buffer.insert(begin(buffer) + insert_index, current_step);
    return insert_index;
}

std::pair<Buffer, std::size_t> spin(int step_size, int total_steps)
{
    Buffer buffer;
    buffer.reserve(total_steps);
    buffer.push_back(0);
    std::size_t start_index = 0;
    for(int i = 0; i < total_steps; ++i) {
        start_index = spin_step(buffer, step_size, i + 1, start_index);
    }
    return make_pair(buffer, start_index);
}

int spin_after_0(int step_size, int total_steps)
{
    std::size_t insert_index = 0;
    std::size_t buffer_size = 1;
    int ret = 0;
    for(int i = 0; i < total_steps; ++i) {
        insert_index = ((insert_index + step_size) % buffer_size) + 1;
        if(insert_index == 1) { ret = i + 1; }
        ++buffer_size;
    }
    return ret;
}
