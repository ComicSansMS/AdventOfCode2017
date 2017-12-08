#include <program_tree.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
        p.weight = std::stoi(matches[2]);

        if(matches[4].matched) {
            // has dependencies
            using regex_it = std::regex_iterator<std::smatch::value_type::iterator>;
            auto it_begin = regex_it(matches[4].first, matches[4].second, rx_dependency);
            auto it_end = regex_it();
            std::transform(it_begin, it_end, std::back_inserter(p.dependencies),
                           [](auto const& match) { return match[1]; });
        }

        p.total_weight = 0;
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

int calculateNodeWeight(std::vector<Program>& programs,
                        std::unordered_map<std::string, int>& name_to_index,
                        int node_index)
{
    auto& node = programs[node_index];
    int child_weights_acc = 0;
    for(auto const& c : node.dependencies) {
        assert(name_to_index.find(c) != end(name_to_index));
        int const child_index = name_to_index[c];
        auto& child = programs[child_index];
        if(child.total_weight == 0) {
            child.total_weight = calculateNodeWeight(programs, name_to_index, child_index);
        }
        child_weights_acc += child.total_weight;
    }
    return child_weights_acc + node.weight;
}

void calculateTotalWeights(std::vector<Program>& programs)
{
    std::unordered_map<std::string, int> name_to_index;
    for(int i=0; i<programs.size(); ++i) {
        name_to_index[programs[i].name] = i;
    }

    int root = findRoot(programs);
    int root_weight = calculateNodeWeight(programs, name_to_index, root);
    programs[root].total_weight = root_weight;
}

bool childrenAreBalanced(std::vector<Program> const& programs,
                         std::unordered_map<std::string, int>& name_to_index,
                         int node_index)
{
    auto const& node = programs[node_index];
    int expected_weight = 0;
    bool is_first = true;
    for(auto const& c : node.dependencies) {
        auto& child = programs[name_to_index[c]];
        if(is_first) { expected_weight = child.total_weight; is_first = false; }
        if(child.total_weight != expected_weight) { return false; }
    }
    return true;
}

int findWeightMismatch(std::vector<Program> const& programs)
{
    std::unordered_map<std::string, int> name_to_index;
    for(int i=0; i<programs.size(); ++i) {
        name_to_index[programs[i].name] = i;
    }

    for(int i=0; i<programs.size(); ++i) {
        if(!childrenAreBalanced(programs, name_to_index, i)) {
            auto& node = programs[i];
            bool all_grandchildren_balanced = std::all_of(begin(node.dependencies), end(node.dependencies),
                [&](std::string const& c) { return childrenAreBalanced(programs, name_to_index, name_to_index[c]); });
            if(all_grandchildren_balanced) {
                // found defective node: one of our children is wrong
                std::vector<Program> children;
                for(auto const& c : node.dependencies) {
                    children.push_back(programs[name_to_index[c]]);
                }
                assert(children.size() > 2);

                int correct_weight = children.front().total_weight;
                int odd_one = -1;
                for(int i=1; i<children.size(); ++i) {
                    if(children[i].total_weight != correct_weight) {
                        if(odd_one == -1) {
                            odd_one = i;
                        } else {
                            // we found *two* odds, which means, front is actually the odd one
                            odd_one = 0;
                            correct_weight = children[i].total_weight;
                            break;
                        }
                    }
                }

                auto& odds_children = children[odd_one].dependencies;
                int childrens_weight = programs[name_to_index[odds_children.front()]].total_weight * static_cast<int>(odds_children.size());

                return std::abs(correct_weight - childrens_weight);
            }
        }
    }

    return -1;
}
