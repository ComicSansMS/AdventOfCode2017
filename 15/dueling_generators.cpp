#include <dueling_generators.hpp>

#include <cassert>
#include <cstdint>
#include <iterator>
#include <regex>
#include <sstream>
#include <vector>

Generators parseInput(std::string_view input)
{
    std::regex rx_line(R"(^Generator (\w+) starts with (\d+)$)");

    Generators ret = { {-1,-1}, {-1,-1} };
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();
    for(auto it = it_begin; it != it_end; ++it) {
        auto const& match = *it;
        int current_value = std::stoi(match[2]);
        if(match[1].compare("A") == 0) {
            ret.a = { 16807, current_value };
        } else if(match[1].compare("B") == 0) {
            ret.b = { 48271, current_value };
        }
    }
    assert((ret.a.current_value != -1) && (ret.b.current_value != -1));
    return ret;
}

void generate(Generators& gen)
{
    gen.a.current_value = static_cast<std::uint64_t>(gen.a.current_value) * gen.a.factor % 2147483647u;
    gen.b.current_value = static_cast<std::uint64_t>(gen.b.current_value) * gen.b.factor % 2147483647u;
}

void generate_picky(Generators& gen)
{
    do {
        gen.a.current_value = static_cast<std::uint64_t>(gen.a.current_value) * gen.a.factor % 2147483647u;
    } while((gen.a.current_value % 4) != 0);
    do {
        gen.b.current_value = static_cast<std::uint64_t>(gen.b.current_value) * gen.b.factor % 2147483647u;
    } while((gen.b.current_value % 8) != 0);
}

bool does_match(Generators const& gen)
{
    int const mask = 0xffff;
    return static_cast<std::uint16_t>(gen.a.current_value & mask) == static_cast<std::uint16_t>(gen.b.current_value & mask);
}
