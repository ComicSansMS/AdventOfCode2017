#include <program_tree.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

std::vector<Program> parseInput(std::string_view input)
{
    std::vector<Program> ret;
    std::stringstream sstr{std::string(input)};

    std::regex rx_program(R"(^([a-z]+) \((\d+)\)( -> ([a-z, ]+))?$)");
    std::regex rx_dependency(R"(([a-z]+)(, )?)");

    std::string line;
    while(std::getline(sstr, line)) {
        std::smatch matches;
        bool does_match = std::regex_search(line, matches, rx_program);
        assert(does_match);
        Program p;
        p.name = matches[1];
        p.id = std::stoi(matches[2]);

        if(matches[4].matched) {
            // has dependencies
            using regex_it = std::regex_iterator<std::smatch::value_type::iterator>;
            auto it_begin = regex_it(matches[4].first, matches[4].second, rx_dependency);
            auto it_end = regex_it();
            std::transform(it_begin, it_end, std::back_inserter(p.dependencies),
                           [](auto const& match) { return match[1]; });
        }

        ret.push_back(p);
    }
    return ret;
}

void determineParents(std::vector<Program>& programs)
{
    std::unordered_map<std::string, int> name_to_index;
    for(int i=0; i<programs.size(); ++i) {
        name_to_index[programs[i].name] = i;
    }

    for(auto const& p : programs) {
        for(auto const& d : p.dependencies) {
            auto& dep_program = programs[name_to_index[d]];
            assert(dep_program.parent.empty());
            dep_program.parent = p.name;
        }
    }
}

int findRoot(std::vector<Program>& programs)
{
    auto it = std::find_if(begin(programs), end(programs), [](Program const& p) { return p.parent.empty(); });
    assert(it != end(programs));
    return static_cast<int>(std::distance(begin(programs), it));
}

