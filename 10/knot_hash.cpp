#include <knot_hash.hpp>

#include <cassert>
#include <iterator>
#include <limits>
#include <regex>
#include <utility>

std::vector<int> parseLengths(std::string_view input)
{
    std::vector<int> ret;
    std::regex rx(R"(([\d]+)[,]?)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret),
                   [](auto const& match) { return std::stoi(match[1]); });

    return ret;
}

void circularReverse(StringCircle& sc, int start_index, int length)
{
    int end_index = start_index + length - 1;
    assert(sc.size() <= std::numeric_limits<int>::max());
    int const sc_length = static_cast<int>(sc.size());
    while(start_index < end_index) {
        std::swap(sc[start_index % sc_length], sc[end_index % sc_length]);
        ++start_index;
        --end_index;
    }
}

void knotHash(StringCircle& sc, std::vector<int> const& lengths)
{
    int current_position = 0;
    int skip_size = 0;
    for(auto const& l : lengths) {
        circularReverse(sc, current_position, l);
        current_position += l + skip_size;
        ++skip_size;
    }
}

