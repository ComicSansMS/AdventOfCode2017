#include <disk_defrag.hpp>
#include <knot_hash.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

void setBitsetFromHash(std::bitset<128>& bs, std::string const& hash)
{
    std::size_t col = 0;
    for(auto const& d : hash) {
        std::size_t const numeric_value = (d >= '0' && d <= '9') ? (d - '0') : ((d - 'a') + 10);
        assert(numeric_value <= 0xf);
        for(std::size_t i=0; i<4; ++i) {
            std::size_t mask = (std::size_t{1} << (std::size_t{3} - i));
            if((numeric_value & mask) != 0) {
                bs[col].flip();
            }
            ++col;
        }
    }
}

Grid::Grid(std::string_view generator)
{
    std::string row_generator;
    row_generator.reserve(generator.size() + 5);
    for(std::size_t row=0; row<m_rows.size(); ++row) {
        row_generator = generator;
        row_generator += "-";
        row_generator += std::to_string(row);
        auto const row_hash = knotHash(row_generator);
        setBitsetFromHash(m_rows[row], row_hash);
    }
}

bool Grid::get(std::size_t row, std::size_t col) const
{
    return m_rows[row][col];
}

std::size_t Grid::count() const
{
    return std::accumulate(begin(m_rows), end(m_rows), std::size_t{0},
                           [](std::size_t acc, auto const& bs) { return acc + bs.count(); });
}

GridData Grid::getGridData() const
{
    return m_rows;
}

namespace
{
struct Coordinates {
    std::uint8_t r;
    std::uint8_t c;

    Coordinates(int nr, int nc) :r(static_cast<uint8_t>(nr)), c(static_cast<std::uint8_t>(nc))
    {
        assert((nr >= 0) && (nr < 128));
        assert((nc >= 0) && (nc < 128));
    }
};
}

void floodFill(GridData& g, std::size_t start_row, std::size_t start_col)
{
    std::vector<Coordinates> stack;
    stack.push_back(Coordinates(static_cast<int>(start_row), static_cast<int>(start_col)));
    while(!stack.empty()) {
        auto node = stack.back();
        stack.pop_back();
        if(g[node.r][node.c]) {
            g[node.r][node.c].flip();
        }
        // left neighbor
        if((node.r > 0) && (g[node.r - 1][node.c])) {
            stack.emplace_back(node.r - 1, node.c);
        }
        // right neighbor
        if((node.r < 127) && (g[node.r + 1][node.c])) {
            stack.emplace_back(node.r + 1, node.c);
        }
        // top neighbor
        if((node.c > 0) && (g[node.r][node.c - 1])) {
            stack.emplace_back(node.r, node.c - 1);
        }
        // bottom neighbor
        if((node.c < 127) && (g[node.r][node.c + 1])) {
            stack.emplace_back(node.r, node.c + 1);
        }
    }
}

std::size_t Grid::countRegions() const
{
    auto g = m_rows;
    // as soon as we find a 1 element, flood fill the region with 0s.
    // continue until no more 1 can be found
    std::size_t n_regions = 0;
    for(;;) {
        std::optional<Coordinates> candidate;
        for(std::uint8_t ir = 0; ir < 128; ++ir) {
            for(std::uint8_t ic = 0; ic < 128; ++ic) {
                if(g[ir][ic]) {
                    candidate = Coordinates{ ir, ic };
                }
            }
        }
        if(!candidate) { break; }
        floodFill(g, candidate->r, candidate->c);
        ++n_regions;
    }
    return n_regions;
}

