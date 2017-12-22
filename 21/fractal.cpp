#include <fractal.hpp>

#include <cassert>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>

Rules parseInput(std::string_view input)
{
    Rules ret;

    std::regex rx_2x2(R"(^([\.#]+)/([\.#]+) => ([\.#]+)/([\.#]+)/([\.#]+)$)");
    std::regex rx_3x3(R"(^([\.#]+)/([\.#]+)/([\.#]+) => ([\.#]+)/([\.#]+)/([\.#]+)/([\.#]+)$)");

    std::stringstream sstr(std::string{input});
    std::string line;
    auto convert = [](char c) -> bool { return (c == '#') ? 1 : 0; };
    while(std::getline(sstr, line)) {
        std::smatch matches;
        if(std::regex_match(line, matches, rx_2x2)) {
            Rule2x2 r;
            auto const str_2x2 = matches[1].str() + matches[2].str();
            assert(str_2x2.length() == 4);
            std::transform(begin(str_2x2), end(str_2x2), begin(r.match[0]), convert);
            auto const str_3x3 = matches[3].str() + matches[4].str() + matches[5].str();
            assert(str_3x3.length() == 9);
            std::transform(begin(str_3x3), end(str_3x3), begin(r.replacement), convert);
            ret.rules2x2.push_back(r);
        } else {
            std::regex_match(line, matches, rx_3x3);
            Rule3x3 r;
            auto const str_3x3 = matches[1].str() + matches[2].str() + matches[3].str();
            assert(str_3x3.length() == 9);
            std::transform(begin(str_3x3), end(str_3x3), begin(r.match[0]), convert);

            auto const str_4x4 = matches[4].str() + matches[5].str() + matches[6].str() + matches[7].str();
            assert(str_4x4.length() == 16);
            std::transform(begin(str_4x4), end(str_4x4), begin(r.replacement), convert);
            ret.rules3x3.push_back(r);
        }
    }
    addTransformedRules(ret);
    return ret;
}


Canvas::Canvas()
    :m_field{ false, true, false, false, false, true, true, true, true }, m_width(3)
{
    // .#.
    // ..#
    // ###
}

Canvas::Canvas(std::vector<char> const& field, std::size_t width)
    :m_field(field), m_width(width)
{
    assert(m_field.size() == m_width * m_width);
    assert((m_width % 2 == 0) || (m_width % 3 == 0));
}

std::size_t Canvas::getWidth() const
{
    return m_width;
}

bool Canvas::getField(std::size_t x, std::size_t y) const
{
    assert(y < m_width);
    assert(x < m_field.size() / m_width);
    return (m_field[y*m_width + x] != 0);
}

void Canvas::setField(std::size_t x, std::size_t y, bool b)
{
    assert(y < m_width);
    assert(x < m_field.size() / m_width);
    m_field[y*m_width + x] = b;
}

SubField2x2 Canvas::getSubField2x2(std::size_t x, std::size_t y)
{
    return getSubFieldNxN<2>(x, y);
}

SubField3x3 Canvas::getSubField3x3(std::size_t x, std::size_t y)
{
    return getSubFieldNxN<3>(x, y);
}

SubField4x4 Canvas::getSubField4x4(std::size_t x, std::size_t y)
{
    return getSubFieldNxN<4>(x, y);
}

Canvas Canvas::applyRules(Rules const& rules)
{
    if((m_width % 2) == 0) {
        auto const n_blocks = m_width / 2;
        auto const new_width = n_blocks * 3;
        Canvas ret(std::vector<char>(new_width*new_width, 0), new_width);
        for(std::size_t iy=0; iy<n_blocks; ++iy) {
            for(std::size_t ix=0; ix<n_blocks; ++ix) {
                auto source = getSubField2x2(ix, iy);
                auto it_match = std::find_if(begin(rules.rules2x2), end(rules.rules2x2),
                                             [source](auto const& r) { return doesMatch(r, source); });
                assert(it_match != end(rules.rules2x2));
                auto target = ret.getSubField3x3(ix, iy);
                for(std::size_t iiy=0; iiy<3; ++iiy) {
                    for(std::size_t iix=0; iix<3; ++iix) {
                        target.setField(iix, iiy, (it_match->replacement[iiy*3 + iix] != 0));
                    }
                }
            }
        }
        return ret;
    } else {
        assert((m_width % 3) == 0);
        auto const n_blocks = m_width / 3;
        auto const new_width = n_blocks * 4;
        Canvas ret(std::vector<char>(new_width*new_width, 0), new_width);
        for(std::size_t iy=0; iy<n_blocks; ++iy) {
            for(std::size_t ix=0; ix<n_blocks; ++ix) {
                auto source = getSubField3x3(ix, iy);
                auto it_match = std::find_if(begin(rules.rules3x3), end(rules.rules3x3),
                                             [source](auto const& r) { return doesMatch(r, source); });
                assert(it_match != end(rules.rules3x3));
                auto target = ret.getSubField4x4(ix, iy);
                for(std::size_t iiy=0; iiy<4; ++iiy) {
                    for(std::size_t iix=0; iix<4; ++iix) {
                        target.setField(iix, iiy, (it_match->replacement[iiy*4 + iix] != 0));
                    }
                }
            }
        }
        return ret;
    }
}

std::size_t Canvas::pegCount() const
{
    return std::count(begin(m_field), end(m_field), 1);
}

std::ostream& operator<<(std::ostream& os, Canvas const& c) {
    for(std::size_t iy=0; iy<c.getWidth(); ++iy) {
        for(std::size_t ix=0; ix<c.getWidth(); ++ix) {
            os << (c.getField(ix, iy) ? '#' : '.');
        }
        os << '\n';
    }
    return os;
}

namespace {
struct Transform {
    std::array<int, 4> m;
};

inline Transform constexpr Ident() { return Transform{1, 0, 0, 1}; }
inline Transform constexpr Rot90() { return Transform{ 0, 1, -1, 0 }; }
inline Transform constexpr Rot180() { return Transform{ -1, 0, 0, -1 }; }
inline Transform constexpr Rot270() { return Transform{ 0, -1, 1, 0 }; }
inline Transform constexpr FlipH() { return Transform{ -1, 0, 0, 1 }; }
inline Transform constexpr FlipV() { return Transform{ 1, 0, 0, -1 }; }
inline Transform constexpr FlipRot90() { return Transform{ 0, -1, -1, 0 }; }
inline Transform constexpr FlipRot270() { return Transform{ 0, 1, 1, 0 }; }

inline auto constexpr getTransforms() { return std::array<Transform, 8>{ Ident(), Rot90(), Rot180(), Rot270(), FlipH(), FlipV(), FlipRot90(), FlipRot270() }; }
}

void addTransformedRules(Rules& r)
{
    auto constexpr transforms = getTransforms();
    // 2x2
    {
        auto const apply_transform = [](Transform const& t, int ix, int iy) {
            auto const x = (ix == 0) ? -1 : 1;
            auto const y = (iy == 0) ? 1 : -1;
            return std::make_pair( ((x*t.m[0] + y*t.m[1]) == 1) ? 1 : 0,
                                   ((x*t.m[2] + y*t.m[3]) == 1) ? 0 : 1);
        };

        for(auto& r2 : r.rules2x2) {
            std::size_t i = 0;
            for(auto const& t : transforms) {
                for(int iy = 0; iy < 2; ++iy) {
                    for(int ix = 0; ix < 2; ++ix) {
                        auto const [itx, ity] = apply_transform(t, ix, iy);
                        r2.match[i][ity*2 + itx] = r2.match[0][iy*2 + ix];
                    }
                }
                ++i;
            }
        }
    }
    // 3x3
    {
        auto const apply_transform = [](Transform const& t, int ix, int iy) {
            auto const x = (ix - 1);
            auto const y = (1 - iy);
            return std::make_pair((x*t.m[0] + y*t.m[1]) + 1,
                                  1 - (x*t.m[2] + y*t.m[3]));
        };

        for(auto& r3 : r.rules3x3) {
            std::size_t i = 0;
            for(auto const& t : transforms) {
                for(int iy = 0; iy < 3; ++iy) {
                    for(int ix = 0; ix < 3; ++ix) {
                        auto const [itx, ity] = apply_transform(t, ix, iy);
                        r3.match[i][ity*3 + itx] = r3.match[0][iy*3 + ix];
                    }
                }
                ++i;
            }
        }
    }
}
