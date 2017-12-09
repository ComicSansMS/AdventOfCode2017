#include <garbage_stream.hpp>

#include <cassert>

StateMachine::StateMachine()
    :m_state(Empty{}), m_groupDepth(0), m_score(0), m_ignoreNextChar(false), m_garbageCount(0)
{
}

void StateMachine::Empty::process(char c, StateMachine& sm)
{
    switch(c) {
    case '{': sm.increaseGroupDepth(); break;
    default: throw unexpected_token{};
    }
}

void StateMachine::InGroup::process(char c, StateMachine& sm)
{
    switch(c) {
    case '{':
        sm.increaseGroupDepth();
        break;
    case '}':
        sm.decreaseGroupDepth();
        break;
    case ',':
        // next element
        break;
    case '<':
        sm.switchState(InGarbage{});
        break;
    default: throw unexpected_token{};
    }
}

void StateMachine::InGarbage::process(char c, StateMachine& sm)
{
    switch(c)
    {
    default: sm.countGarbage(); break;
    case '>': sm.switchState(InGroup{}); break;
    case '!': sm.ignoreNextChar(); break;
    }
}

void StateMachine::process(char c)
{
    if(m_ignoreNextChar) { m_ignoreNextChar = false; return; }
    std::visit([c, this](auto s) { s.process(c, *this); }, m_state);
}

void StateMachine::switchState(State s)
{
    m_state = s;
}

bool StateMachine::inAcceptingState() const
{
    return m_state.index() == 0;
}

void StateMachine::increaseGroupDepth()
{
    ++m_groupDepth;
    m_state = InGroup{};
}

void StateMachine::decreaseGroupDepth()
{
    m_score += m_groupDepth;
    --m_groupDepth;
    if(m_groupDepth == 0) { m_state = Empty{}; }
}

int StateMachine::getGroupDepth() const
{
    return m_groupDepth;
}

int StateMachine::getScore() const
{
    return m_score;
}

void StateMachine::ignoreNextChar()
{
    m_ignoreNextChar = true;
}

void StateMachine::countGarbage()
{
    ++m_garbageCount;
}

int StateMachine::getGarbageCount() const
{
    return m_garbageCount;
}

StateMachine parseStream(std::string_view input)
{
    StateMachine sm;
    for(auto const& c : input) {
        sm.process(c);
    }
    return sm;
}
