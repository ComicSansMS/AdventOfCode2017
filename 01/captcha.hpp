#ifndef ADVENT_OF_CODE_01_CAPTCHA_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_CAPTCHA_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

/** A Digit is a number in the range [0..9]
 */
class Digit {
    std::uint8_t m_digit;
public:
    /** Construct from a char in the range ['0'..'9']
     */
    Digit(char c);

    int get() const;
};

using DigitSequence = std::vector<Digit>;

DigitSequence parseInput(std::string_view str);

/** Access into the DigitSequence with wrap-around, ie. s[s.size()] == s[0].
*/
Digit const& cyclic_access(DigitSequence const& s, std::size_t index);

/** The first captcha is solved by summing up all digits that are followed by a digit of the same value.
 */
int solveCaptcha(DigitSequence const& input);

/** The second captcha is solved by summing up all digits that have a digit of the same value halfway
 *through the sequence (with wrap-around).
 */
int solveCaptcha2(DigitSequence const& input);

#endif
