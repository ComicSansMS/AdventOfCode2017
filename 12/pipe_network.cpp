#include <pipe_network.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <sstream>
#include <utility>

NodeConnections parseLine(std::string_view input)
{
    std::regex rx_line(R"(^([\d]+) <-> ([\d, ]+)$)");
    std::match_results<std::string_view::iterator> matches;
    bool found = std::regex_match(begin(input), end(input), matches, rx_line);
    assert(found);
    NodeConnections ret;
    ret.node = std::stoi(matches[1].str());

    std::regex rx_connection(R"(([\d]+)(, )?)");
    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(matches[2].first, matches[2].second, rx_connection);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret.connected_nodes),
                   [](auto const& match) { return std::stoi(match[1]); });

    return ret;
}

std::vector<NodeConnections> parseInput(std::string_view input)
{
    std::vector<NodeConnections> ret;
    std::stringstream sstr{std::string(input)};
    std::string line;
    while(std::getline(sstr, line)) {
        ret.push_back(parseLine(line));
    }
    return ret;
}

Graph buildGraph(std::vector<NodeConnections> const& nc)
{
    Graph ret;
    ret.nodes.reserve(nc.size());
    ret.edges.reserve(nc.size());
    for(auto const& c : nc) {
        ret.nodes.push_back(c.node);
        std::unordered_set<int> connected_nodes;
        for(auto const& conn : c.connected_nodes) { connected_nodes.insert(conn); }
        ret.edges.insert(std::make_pair(c.node, connected_nodes));
    }
    return ret;
}

std::vector<int> getConnectedGroup(Graph const& g, int source_node)
{
    std::vector<int> ret;
    ret.push_back(source_node);
    for(std::size_t traverse_index = 0; traverse_index < ret.size(); ++traverse_index)
    {
        auto current_node = ret[traverse_index];
        assert(std::find(begin(g.nodes), end(g.nodes), current_node) != end(g.nodes));
        auto it_neighbourhood = g.edges.find(current_node);
        assert(it_neighbourhood != end(g.edges));
        auto const& neighbourhood = it_neighbourhood->second;
        for(auto const& neighbour : neighbourhood) {
            if(std::find(begin(ret), end(ret), neighbour) == end(ret)) {
                ret.push_back(neighbour);
            }
        }
    }

    std::sort(begin(ret), end(ret));
    return ret;
}

std::vector<std::vector<int>> getAllConnectedGroups(Graph const& g)
{
    auto nodes = g.nodes;
    std::vector<std::vector<int>> groups;
    while(!nodes.empty()) {
        auto current_node = nodes.back();
        auto current_group = getConnectedGroup(g, current_node);
        nodes.erase(std::remove_if(begin(nodes), end(nodes),
                    [&current_group](auto const n) {
                        return std::find(begin(current_group), end(current_group), n) != end(current_group);
                    }), end(nodes));
        groups.emplace_back(std::move(current_group));
    }
    return groups;
}
