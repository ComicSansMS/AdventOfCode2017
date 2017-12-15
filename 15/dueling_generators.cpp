#include <dueling_generators.hpp>

#include <cassert>
#include <cstdint>
#include <iterator>
#include <regex>
#include <sstream>
#include <vector>

std::vector<Generator> parseInput(std::string_view input)
{
    std::regex rx_line(R"(^Generator (\w+) starts with (\d+)$)");

    std::vector<Generator> ret;

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret), [](auto const& match) {
        int factor = -1;
        if(match[1].compare("A") == 0) {
            factor = 16807;
        } else if(match[1].compare("B") == 0) {
            factor = 48271;
        }
        assert(factor != -1);
        int current_value = std::stoi(match[2]);
        return Generator{ factor, current_value };
    });
    return ret;
}

void generate(std::vector<Generator>& gen)
{
    for(auto& g : gen) {
        g.current_value = static_cast<std::uint64_t>(g.current_value) * g.factor % 2147483647u;
    }
}

bool does_match(std::vector<Generator> const& gen)
{
    assert(gen.size() > 1);
    auto it = begin(gen);
    int const mask = 0xffff;
    std::uint16_t const expected = static_cast<std::uint16_t>(it->current_value & mask);
    return std::all_of(it + 1, end(gen), [mask, expected](Generator const& g) { 
        return static_cast<std::uint16_t>(g.current_value & mask) == expected;
    });
}
