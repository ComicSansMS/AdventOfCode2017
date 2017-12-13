#ifndef ADVENT_OF_CODE_13_FIREWALL_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_FIREWALL_HPP_INCLUDE_GUARD

#include <string_view>

#include <optional>
#include <vector>

struct Firewall {
    struct Scanner {
        int range;
        int position;
        enum class Direction { Up, Down } direction;
    };
    std::vector<std::optional<Scanner>> scanners;
    int player_position;
};

Firewall parseInput(std::string_view input);

enum class Player {
    Move,
    Stop
};

bool simulateStep(Firewall& firewall, Player move_player);

struct TraversalResult {
    bool was_caught;
    int penalty;
};

TraversalResult simulateTraversal(int initial_wait, Firewall& firewall);

bool simulateTraversal_fast(Firewall& firewall);

int findSmallestDelay(Firewall const& firewall);

#endif
