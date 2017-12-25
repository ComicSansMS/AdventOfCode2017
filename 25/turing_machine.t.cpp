#include <catch.hpp>

#include <turing_machine.hpp>


TEST_CASE("Turing Machine")
{
    char const* input = R"(Begin in state A.
Perform a diagnostic checksum after 6 steps.

In state A:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state B.
  If the current value is 1:
    - Write the value 0.
    - Move one slot to the left.
    - Continue with state B.

In state B:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state A.
  If the current value is 1:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state A.
)";

    SECTION("Parse input")
    {
        auto const tm = parseInput(input);
        CHECK(tm.step_count == 0);
        CHECK(tm.end_step == 6);
        REQUIRE(tm.states.size() == 2);
        CHECK(tm.states[tm.current_state].name == 'A');

        CHECK(tm.states[0].name == 'A');
        CHECK(tm.states[0].transitions[0].write == 1);
        CHECK(tm.states[0].transitions[0].move == Direction::Right);
        CHECK(tm.states[tm.states[0].transitions[0].next_state].name == 'B');
        CHECK(tm.states[0].transitions[1].write == 0);
        CHECK(tm.states[0].transitions[1].move == Direction::Left);
        CHECK(tm.states[tm.states[0].transitions[1].next_state].name == 'B');

        CHECK(tm.states[1].name == 'B');
        CHECK(tm.states[1].transitions[0].write == 1);
        CHECK(tm.states[1].transitions[0].move == Direction::Left);
        CHECK(tm.states[tm.states[1].transitions[0].next_state].name == 'A');
        CHECK(tm.states[1].transitions[1].write == 1);
        CHECK(tm.states[1].transitions[1].move == Direction::Right);
        CHECK(tm.states[tm.states[1].transitions[1].next_state].name == 'A');

        CHECK(tm.tape_position == 0);
        CHECK(tm.tape.getBeginIndex() == 0);
        CHECK(tm.tape.getEndIndex() == 0);
    }

    SECTION("Tape")
    {
        Tape t;
        for(int i=0; i<10; ++i) {
            t[i-5] = i + 1;
        }
        CHECK(t.getBeginIndex() == -5);
        CHECK(t.getEndIndex() == 5);
        CHECK(t[-5] == 1);
        CHECK(t[4] == 10);

        CHECK(t[100] == 0);
    }

    SECTION("Stepwise execution")
    {
        auto tm = parseInput(input);

        CHECK(tm.tape_position == 0);
        step(tm);
        CHECK(tm.states[tm.current_state].name == 'B');
        CHECK(tm.tape[0] == 1);
        CHECK(tm.tape_position == 1);

        step(tm);
        CHECK(tm.states[tm.current_state].name == 'A');
        CHECK(tm.tape[0] == 1);
        CHECK(tm.tape[1] == 1);
        CHECK(tm.tape_position == 0);

        step(tm);
        CHECK(tm.states[tm.current_state].name == 'B');
        CHECK(tm.tape[0] == 0);
        CHECK(tm.tape[1] == 1);
        CHECK(tm.tape_position == -1);

        step(tm);
        CHECK(tm.states[tm.current_state].name == 'A');
        CHECK(tm.tape[-1] == 1);
        CHECK(tm.tape[0] == 0);
        CHECK(tm.tape[1] == 1);
        CHECK(tm.tape_position == -2);

        step(tm);
        CHECK(tm.states[tm.current_state].name == 'B');
        CHECK(tm.tape[-2] == 1);
        CHECK(tm.tape[-1] == 1);
        CHECK(tm.tape[0] == 0);
        CHECK(tm.tape[1] == 1);
        CHECK(tm.tape_position == -1);
    }

    SECTION("Execution")
    {
        auto tm = parseInput(input);

        int const checksum = run(tm);

        CHECK(tm.step_count == tm.end_step);
        CHECK(tm.states[tm.current_state].name == 'A');
        CHECK(tm.tape_position == 0);

        CHECK(checksum == 3);
    }
}
