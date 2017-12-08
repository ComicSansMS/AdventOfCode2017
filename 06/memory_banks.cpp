#include <memory_banks.hpp>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>

MemoryArray parseInput(std::string_view input)
{
    MemoryArray ret;
    std::stringstream sstr{std::string(input)};
    std::copy(std::istream_iterator<int>(sstr), std::istream_iterator<int>(), std::back_inserter(ret));
    return ret;
}

std::string banksToString(MemoryArray const& banks)
{
    std::stringstream sstr;
    bool first = true;
    for(auto const& i : banks) {
        if(!first) { sstr << "-"; }
        sstr << i;
        first = false;
    }
    return sstr.str();
}

void iterate(MemoryArray& banks)
{
    auto it = std::max_element(begin(banks), end(banks));
    int i = *it;
    *it = 0;
    for(; i > 0; --i) {
        ++it;
        if(it == end(banks)) { it = begin(banks); }
        ++(*it);
    }
}

int redistribute(MemoryArray& banks)
{
    std::unordered_set<std::string> seen;
    for(int ret = 0; ; ++ret) {
        auto str = banksToString(banks);
        if(seen.find(str) != end(seen)) { return ret; }
        seen.emplace(std::move(str));
        iterate(banks);
    }
    return 0;
}

