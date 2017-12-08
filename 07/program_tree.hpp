#ifndef ADVENT_OF_CODE_07_PROGRAM_TREE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_PROGRAM_TREE_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <vector>

struct Program {
    int weight;
    std::string name;
    std::vector<std::string> dependencies;
    std::string parent;
    int total_weight;
};

std::vector<Program> parseInput(std::string_view input);

void determineParents(std::vector<Program>& programs);

int findRoot(std::vector<Program>& programs);

void calculateTotalWeights(std::vector<Program>& programs);

int findWeightMismatch(std::vector<Program> const& programs);


#endif
