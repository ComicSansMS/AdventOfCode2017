#ifndef ADVENT_OF_CODE_14_DISK_DEFRAG_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_DISK_DEFRAG_HPP_INCLUDE_GUARD

#include <string_view>

#include <array>
#include <bitset>

void setBitsetFromHash(std::bitset<128>& bs, std::string const& hash);

using GridData = std::array<std::bitset<128>, 128>;

class Grid {
private:
    GridData m_rows;
public:
    Grid(std::string_view generator);

    bool get(std::size_t row, std::size_t col) const;

    std::size_t count() const;

    std::size_t countRegions() const;

    GridData getGridData() const;
};

void floodFill(GridData& g, std::size_t start_row, std::size_t start_col);

#endif
