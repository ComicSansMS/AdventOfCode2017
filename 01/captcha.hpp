#ifndef ADVENT_OF_CODE_01_CAPTCHA_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_CAPTCHA_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

class Digit {
    std::uint8_t m_digit;
public:
    Digit(char c);

    int get() const;
};

using DigitSequence = std::vector<Digit>;

DigitSequence parseInput(std::string_view str);

int solveCaptcha(DigitSequence const& input);

#endif
