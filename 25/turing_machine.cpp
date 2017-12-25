#include <turing_machine.hpp>

#include <cassert>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

TuringMachine parseInput(std::string_view input)
{
    TuringMachine ret;
    ret.step_count = 0;
    ret.tape_position = 0;

    std::stringstream sstr(std::string{input});
    std::string line;
    bool res = static_cast<bool>( std::getline(sstr, line) );
    assert(res);
    std::regex rx_first_line(R"(^Begin in state (\w)\.$)");
    std::smatch matches;
    res = std::regex_match(line, matches, rx_first_line);
    assert(res);
    char const starting_state = matches[1].first[0];

    res = static_cast<bool>( std::getline(sstr, line) );
    assert(res);
    std::regex rx_second_line(R"(^Perform a diagnostic checksum after (\d+) steps\.$)");
    res = std::regex_match(line, matches, rx_second_line);
    assert(res);
    ret.end_step = std::stoi(matches[1]);

    std::regex rx_state_header(R"(^In state (\w):$)");
    std::regex rx_state_cond(R"(^  If the current value is (\d):$)");
    std::regex rx_state_write(R"(^    - Write the value (\d)\.$)");
    std::regex rx_state_move(R"(^    - Move one slot to the (\w+)\.$)");
    std::regex rx_state_next(R"(^    - Continue with state (\w)\.$)");
    std::unordered_map<char, std::size_t> state_index_map;
    struct TempTransitions {
        char state[2];
    };
    std::vector<TempTransitions> temp_transitions;
    for(;;) {
        res = static_cast<bool>( std::getline(sstr, line) );
        if(!res) { break; }
        assert(line.empty());
        res = static_cast<bool>( std::getline(sstr, line) );
        assert(res);
        res = std::regex_match(line, matches, rx_state_header);
        assert(res);
        State s;
        s.name = matches[1].first[0];

        temp_transitions.push_back(TempTransitions{});
        for(int i=0; i<2; ++i) {
            res = static_cast<bool>( std::getline(sstr, line) );
            assert(res);
            res = std::regex_match(line, matches, rx_state_cond);
            assert(res);
            int const cond_s = std::stoi(matches[1]);
            assert(cond_s == i);

            res = static_cast<bool>( std::getline(sstr, line) );
            assert(res);
            res = std::regex_match(line, matches, rx_state_write);
            assert(res);
            int const write_s = std::stoi(matches[1]);
            assert((write_s == 0) || (write_s == 1));
            s.transitions[i].write = write_s;

            res = static_cast<bool>( std::getline(sstr, line) );
            assert(res);
            res = std::regex_match(line, matches, rx_state_move);
            assert(res);
            if(matches[1].compare("left") == 0) {
                s.transitions[i].move = Direction::Left;
            } else {
                assert(matches[1].compare("right") == 0);
                s.transitions[i].move = Direction::Right;
            }

            res = static_cast<bool>( std::getline(sstr, line) );
            assert(res);
            res = std::regex_match(line, matches, rx_state_next);
            assert(res);
            temp_transitions.back().state[i] = matches[1].first[0];
        }
        assert(state_index_map.find(s.name) == end(state_index_map));
        state_index_map[s.name] = ret.states.size();
        ret.states.push_back(s);
    }

    for(std::size_t i = 0; i < ret.states.size(); ++i) {
        assert(state_index_map.find(temp_transitions[i].state[0]) != end(state_index_map));
        assert(state_index_map.find(temp_transitions[i].state[1]) != end(state_index_map));
        ret.states[i].transitions[0].next_state = state_index_map[temp_transitions[i].state[0]];
        ret.states[i].transitions[1].next_state = state_index_map[temp_transitions[i].state[1]];
    }
    assert(state_index_map.find(starting_state) != end(state_index_map));
    ret.current_state = state_index_map[starting_state];

    return ret;
}

int Tape::getBeginIndex() const
{
    return -static_cast<int>(left.size());
}

int Tape::getEndIndex() const
{
    return static_cast<int>(right.size());
}

int& Tape::operator[](int index)
{
    if(index < 0) {
        if(static_cast<int>(left.size()) < -(index)) {
            left.resize(-(index), 0);
        }
        return left[-(index + 1)];
    } else {
        if(static_cast<int>(right.size()) < index + 1) {
            right.resize(index + 1);
        }
        return right[index];
    }
}

void step(TuringMachine& tm)
{
    auto const& state = tm.states[tm.current_state];
    auto const input = tm.tape[tm.tape_position];
    assert((input == 0) || (input == 1));
    auto const& transition = state.transitions[input];
    tm.tape[tm.tape_position] = transition.write;
    tm.tape_position += (transition.move == Direction::Left) ? (-1) : (+1);
    tm.current_state = transition.next_state;
}

int run(TuringMachine& tm)
{
    for(; tm.step_count < tm.end_step; ++tm.step_count) {
        step(tm);
    }
    int ret = 0;
    for(int i=tm.tape.getBeginIndex(); i<tm.tape.getEndIndex(); ++i) {
        if(tm.tape[i] == 1) { ++ret; }
    }
    return ret;
}
