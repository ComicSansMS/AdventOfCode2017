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

Digit const& cyclic_access(DigitSequence const& s, std::size_t index)
{
    return s[index % s.size()];
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

int solveCaptcha2(DigitSequence const& input)
{
    auto const length = input.size();
    int acc = 0;
    for(std::size_t i = 0; i < length; ++i) {
        if(input[i].get() == cyclic_access(input, i + length/2).get()) {
            acc += input[i].get();
        }
    }
    return acc;
}

