#ifndef ADVENT_OF_CODE_21_FRACTAL_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_21_FRACTAL_HPP_INCLUDE_GUARD

#include <array>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <string_view>
#include <variant>
#include <vector>

template<std::size_t N>
struct RuleNxN {
    std::array<std::array<char, N*N>, 8> match;
    std::array<char, (N+1)*(N+1)> replacement;
};

using Rule2x2 = RuleNxN<2>;
using Rule3x3 = RuleNxN<3>;

struct Rules {
    std::vector<Rule2x2> rules2x2;
    std::vector<Rule3x3> rules3x3;
};

Rules parseInput(std::string_view input);

template<std::size_t N>
struct SubFieldNxN;
using SubField2x2 = SubFieldNxN<2>;
using SubField3x3 = SubFieldNxN<3>;
using SubField4x4 = SubFieldNxN<4>;

class Canvas {
private:
    std::vector<char> m_field;
    std::size_t m_width;
public:
    Canvas();

    Canvas(std::vector<char> const& field, std::size_t width);

    std::size_t getWidth() const;
    bool getField(std::size_t x, std::size_t y) const;
    void setField(std::size_t x, std::size_t y, bool b);

    template<std::size_t N>
    SubFieldNxN<N> getSubFieldNxN(std::size_t x, std::size_t y)
    {
        assert(x*N < m_width);
        assert(y*N < m_width);
        return SubFieldNxN<N>{ this, x*N, y*N };
    }

    SubField2x2 getSubField2x2(std::size_t x, std::size_t y);
    SubField3x3 getSubField3x3(std::size_t x, std::size_t y);
    SubField4x4 getSubField4x4(std::size_t x, std::size_t y);

    Canvas applyRules(Rules const& rules);

    std::size_t pegCount() const;
};

std::ostream& operator<<(std::ostream& os, Canvas const& c);

template<std::size_t N>
struct SubFieldNxN {
    Canvas* canvas;
    std::size_t offset_x;
    std::size_t offset_y;

    bool getField(std::size_t x, std::size_t y) const {
        assert(x < N);
        assert(y < N);
        return canvas->getField(x + offset_x, y + offset_y);
    }

    void setField(std::size_t x, std::size_t y, bool b) {
        assert(x < N);
        assert(y < N);
        return canvas->setField(x + offset_x, y + offset_y, b);
    }
};

void addTransformedRules(Rules& r);

template<std::size_t N>
inline bool doesMatch(RuleNxN<N> const& r, SubFieldNxN<N> const& f)
{
    for(auto const& tr : r.match) {
        bool does_match = true;
        for(std::size_t iy=0; iy<N; ++iy) {
            for(std::size_t ix=0; ix<N; ++ix) {
                if((tr[iy*N + ix] == 1) != f.getField(ix, iy)) {
                    does_match = false;
                }
            }
            if(!does_match) { break; }
        }
        if(does_match) { return true; }
    }
    return false;
}

#endif
