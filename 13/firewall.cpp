#include <firewall.hpp>

#include <cassert>
#include <regex>
#include <sstream>
#include <string>

Firewall parseInput(std::string_view input)
{
    Firewall ret;
    ret.player_position = -1;

    std::regex rx_line(R"(^(\d+): (\d+)$)");

    std::stringstream sstr{std::string(input)};
    std::string line;
    while(std::getline(sstr, line)) {
        std::smatch matches;
        std::regex_match(line, matches, rx_line);
        int current_position = std::stoi(matches[1]);
        int range = std::stoi(matches[2]);
        while(ret.scanners.size() < current_position) { ret.scanners.push_back(std::nullopt); }
        Firewall::Scanner scanner;
        scanner.range = range;
        scanner.position = 0;
        scanner.direction = Firewall::Scanner::Direction::Down;
        ret.scanners.push_back(scanner);
    }

    return ret;
}

bool simulateStep(Firewall& firewall, Player move_player)
{
    if(move_player == Player::Move) {
        ++firewall.player_position;
    }
    bool ret = false;
    if((firewall.player_position >= 0) &&
       (static_cast<std::size_t>(firewall.player_position) < firewall.scanners.size()) &&
       (firewall.scanners[firewall.player_position]) &&
       (firewall.scanners[firewall.player_position]->position == 0)) {
        ret = true;
    }
    for(auto& opt_scanner : firewall.scanners) {
        if(opt_scanner) {
            auto& scanner = *opt_scanner;
            if(scanner.direction == Firewall::Scanner::Direction::Down) {
                if(scanner.position == scanner.range - 1) {
                    scanner.direction = Firewall::Scanner::Direction::Up;
                    --scanner.position;
                } else {
                    ++scanner.position;
                }
            } else {
                if(scanner.position == 0) {
                    scanner.direction = Firewall::Scanner::Direction::Down;
                    ++scanner.position;
                } else {
                    --scanner.position;
                }
            }
        }
    }
    return ret;
}

TraversalResult simulateTraversal(int initial_wait, Firewall& firewall)
{
    TraversalResult ret;
    ret.was_caught = false;
    ret.penalty = 0;
    for(int i=0; i<initial_wait; ++i) {
        simulateStep(firewall, Player::Stop);
    }
    int const field_width = static_cast<int>(firewall.scanners.size());
    while(firewall.player_position < field_width) {
        bool was_caught_in_step = simulateStep(firewall, Player::Move);
        if(was_caught_in_step) {
            assert(firewall.scanners[firewall.player_position]);
            ret.was_caught = true;
            ret.penalty += firewall.player_position * firewall.scanners[firewall.player_position]->range;
        }
    }
    return ret;
}

bool simulateTraversal_fast(Firewall& firewall)
{
    int const field_width = static_cast<int>(firewall.scanners.size());
    while(firewall.player_position < field_width) {
        ++firewall.player_position;
        if((firewall.player_position >= 0) &&
            (static_cast<std::size_t>(firewall.player_position) < firewall.scanners.size()) &&
            (firewall.scanners[firewall.player_position]) &&
            (firewall.scanners[firewall.player_position]->position == 0)) {
            return true;
        }

        for(auto& opt_scanner : firewall.scanners) {
            if(opt_scanner) {
                auto& scanner = *opt_scanner;
                if(scanner.direction == Firewall::Scanner::Direction::Down) {
                    if(scanner.position == scanner.range - 1) {
                        scanner.direction = Firewall::Scanner::Direction::Up;
                        --scanner.position;
                    } else {
                        ++scanner.position;
                    }
                } else {
                    if(scanner.position == 0) {
                        scanner.direction = Firewall::Scanner::Direction::Down;
                        ++scanner.position;
                    } else {
                        --scanner.position;
                    }
                }
            }
        }
    }
    return false;
}


int findSmallestDelay(Firewall const& firewall)
{
    int delay = 0;
    auto start_state = firewall;
    for(;; ++delay) {
        auto tmp = start_state;
        if(!simulateTraversal_fast(tmp)) {
            break;
        }
        simulateStep(start_state, Player::Stop);
    }
    return delay;
}
