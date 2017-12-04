#include <captcha.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>

Digit::Digit(char c)
    :m_digit(c - '0')
{
    assert((c >= '0') && (c <= '9'));
}

int Digit::get() const
{
    return m_digit;
}

DigitSequence parseInput(std::string_view str)
{
    DigitSequence ret;
    ret.reserve(str.length());
    std::transform(begin(str), end(str), std::back_inserter(ret), [](auto c) { return Digit(c); });
    return ret;
}

int solveCaptcha(DigitSequence const& input)
{
    int acc = 0;
    auto const it_end = end(input);
    for(auto it = begin(input); it != it_end; ++it) {
        auto it_next = it + 1;
        if(it_next == it_end) { it_next = begin(input); }

        if(it->get() == it_next->get()) {
            acc += it->get();
        }
    }
    return acc;
}

