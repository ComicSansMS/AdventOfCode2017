#include <knot_hash.hpp>

#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
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

std::vector<int> parseLengths_ASCII(std::string_view input)
{
    std::vector<int> ret;
    int const suffix[] = { 17, 31, 73, 47, 23 };
    ret.reserve(input.length() + sizeof(suffix));
    std::transform(begin(input), end(input), std::back_inserter(ret),
                   [](char c) { return static_cast<int>(c); });
    ret.insert(end(ret), std::begin(suffix), std::end(suffix));
    return ret;
}

StringCircle buildInitialCircle()
{
    StringCircle sc(256, 0);
    std::iota(begin(sc), end(sc), 0);
    return sc;
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

void knotHashSingleRound(StringCircle& sc, std::vector<int> const& lengths, KnotHashState& state)
{
    for(auto const& l : lengths) {
        circularReverse(sc, state.current_position, l);
        state.current_position += l + state.skip_size;
        ++state.skip_size;
    }
}

std::vector<int> getDenseHash(StringCircle& sc)
{
    assert(sc.size() % 16 == 0);
    std::vector<int> denseList;
    denseList.reserve(sc.size() / 16);
    assert(sc.size() <= std::numeric_limits<int>::max());
    for(int i=0; i<static_cast<int>(sc.size()); i+=16) {
        int acc = 0;
        for(int j=0; j<16; ++j) {
            acc ^= sc[i+j];
        }
        denseList.push_back(acc);
    }
    return denseList;
}

std::string hexify(std::vector<int> const& dense_hash)
{
    std::string ret;
    ret.reserve(dense_hash.size() * 2);
    for(auto const& i : dense_hash) {
        int low_nibble = i & 0x0f;
        int high_nibble = (i & 0xf0) >> 4;
        auto to_hex = [](int c) -> char {
                assert(c >= 0 && c < 16);
                switch(c) {
                default: return '0' + static_cast<char>(c);
                case 10: return 'a';
                case 11: return 'b';
                case 12: return 'c';
                case 13: return 'd';
                case 14: return 'e';
                case 15: return 'f';
                }
            };
        ret.push_back(to_hex(high_nibble));
        ret.push_back(to_hex(low_nibble));
    }
    return ret;
}

std::string knotHash(std::string_view input)
{
    auto lengths = parseLengths_ASCII(input);
    StringCircle sc = buildInitialCircle();
    KnotHashState state;
    state.current_position = 0;
    state.skip_size = 0;
    for(int i=0; i<64; ++i) {
        knotHashSingleRound(sc, lengths, state);
    }
    return hexify(getDenseHash(sc));
}

