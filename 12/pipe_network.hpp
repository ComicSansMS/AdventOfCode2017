#ifndef ADVENT_OF_CODE_12_PIPE_NETWORK_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_PIPE_NETWORK_HPP_INCLUDE_GUARD

#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct NodeConnections {
    int node;
    std::vector<int> connected_nodes;
};

NodeConnections parseLine(std::string_view input);

std::vector<NodeConnections> parseInput(std::string_view input);

struct Graph {
    std::vector<int> nodes;
    std::unordered_map<int, std::unordered_set<int>> edges;
};

Graph buildGraph(std::vector<NodeConnections> const& nc);

std::vector<int> getConnectedGroup(Graph const& g, int source_node);

std::vector<std::vector<int>> getAllConnectedGroups(Graph const& g);

#endif
