#ifndef ADVENT_OF_CODE_25_TURING_MACHINE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_25_TURING_MACHINE_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

enum class Direction {
    Left, Right
};

struct State {
    char name;
    struct Transition {
        int write;
        Direction move;
        std::size_t next_state;
    } transitions[2];
};

struct Tape {
    std::vector<int> left;
    std::vector<int> right;

    int getBeginIndex() const;
    int getEndIndex() const;

    int& operator[](int index);
};

struct TuringMachine {
    std::vector<State> states;
    std::size_t current_state;
    std::size_t step_count;
    std::size_t end_step;
    int tape_position;
    Tape tape;
};

TuringMachine parseInput(std::string_view input);

void step(TuringMachine& tm);

int run(TuringMachine& tm);

#endif
