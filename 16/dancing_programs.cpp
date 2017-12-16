#include <dancing_programs.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <string>

DancingStage createDancingStage()
{
    DancingStage ret(16, '\0');
    std::iota(begin(ret), end(ret), 'a');
    return ret;
}

std::string to_string(DancingStage const& stage)
{
    std::string ret;
    ret.reserve(stage.size() + 1);
    std::copy(begin(stage), end(stage), std::back_inserter(ret));
    return ret;
}

Dance parseInput(std::string_view input)
{
    Dance ret;
    for(std::size_t i = 0; i < input.size(); ++i) {
        if(input[i] == 's') {
            auto const arg_start = ++i;
            while((i < input.size()) && (input[i] != ',')) { ++i; }
            int arg = std::stoi(std::string(input.substr(arg_start, (i - arg_start))));
            assert(arg >= 0);
            ret.push_back(Spin{static_cast<std::size_t>(arg)});
        } else if(input[i] == 'x') {
            auto const arg1_start = ++i;
            while(input[i] != '/') { ++i; }
            int arg1 = std::stoi(std::string(input.substr(arg1_start, (i - arg1_start))));
            assert(arg1 >= 0);
            auto const arg2_start = ++i;
            while((i < input.size()) && (input[i] != ',')) { ++i; }
            int arg2 = std::stoi(std::string(input.substr(arg2_start, (i - arg1_start))));
            assert(arg2 >= 0);
            ret.push_back(Exchange{ static_cast<std::size_t>(arg1), static_cast<std::size_t>(arg2) });
        } else if(input[i] == 'p') {
            char arg1 = input[++i];
            ++i;
            assert(input[i] == '/');
            char arg2 = input[++i];
            ret.push_back(Partner{ arg1, arg2 });
        }
    }
    return ret;
}

void perform_move(DancingStage& stage, Spin const& move)
{
    assert(move.x  < stage.size());
    auto it_split = end(stage) - move.x;
    std::rotate(begin(stage), it_split, end(stage));
}

void perform_move(DancingStage& stage, Exchange const& move)
{
    assert((move.pos_a  < stage.size()) && (move.pos_b < stage.size()));
    std::swap(*(begin(stage) + move.pos_a), *(begin(stage) + move.pos_b));
}

void perform_move(DancingStage& stage, Partner const& move)
{
    auto it1 = std::find(begin(stage), end(stage), move.name_a);
    auto it2 = std::find(begin(stage), end(stage), move.name_b);
    assert((it1 != end(stage)) && (it2 != end(stage)));
    std::swap(*it1, *it2);
}

void perform(DancingStage& stage, Dance const& dance)
{
    for(auto const& move : dance) {
        std::visit([&stage](auto m) { perform_move(stage, m); }, move);
    }
}

std::size_t findPeriod(DancingStage& stage, Dance const& dance)
{
    std::unordered_set<std::string> known_positions;
    for(std::size_t i=0; ; ++i) {
        perform(stage, dance);
        auto position = to_string(stage);
        if(known_positions.find(position) == end(known_positions)) {
            known_positions.insert(position);
        } else {
            return i;
        }
    }
}
