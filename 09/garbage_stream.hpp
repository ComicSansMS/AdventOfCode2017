#ifndef ADVENT_OF_CODE_09_GARBAGE_STREAM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_GARBAGE_STREAM_HPP_INCLUDE_GUARD

#include <exception>
#include <string_view>
#include <variant>
#include <vector>

struct unexpected_token : public virtual std::exception {
    char const* what() const noexcept override { return "unexpected token"; }
};

struct StateMachine {
private:
    struct Empty {
        void process(char c, StateMachine& sm);
    };
    struct InGroup {
        void process(char c, StateMachine& sm);
    };
    struct InGarbage {
        void process(char c, StateMachine& sm);
    };
    using State = std::variant<Empty, InGroup, InGarbage>;
    State m_state;
    int m_groupDepth;
    int m_score;
    bool m_ignoreNextChar;
    int m_garbageCount;
public:
    StateMachine();

    void process(char c);

    void switchState(State s);
    bool inAcceptingState() const;

    void increaseGroupDepth();
    void decreaseGroupDepth();
    int getGroupDepth() const;

    int getScore() const;

    void ignoreNextChar();

    void countGarbage();
    int getGarbageCount() const;
};

StateMachine parseStream(std::string_view input);

#endif
